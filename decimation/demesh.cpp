#include "demesh.h"

namespace Decimation {

int DeMesh::contraction(VertexPair P){
	int count = 0;
	DeVertex* A = P.A;
	DeVertex* B = P.B;
	A->setPos(P.mTarget);
	A->merge(B->getQuadMatrix());
	merged[B->getID()] = true;
	vector<DeTriangle*> & Atriangles = A->getAdjacent();
	vector<DeTriangle*> & Btriangles = B->getAdjacent();
	for (DeTriangle* tri : Btriangles)
		if (tri->findVertexID(A->getID()) != NULL)
			tri->setDegeneration(), ++ count;
		else{
			*(tri->findVertexID(B->getID())) = A->getID();
			Atriangles.push_back(tri);
		}
	vector<DeTriangle*> reserve;
	for (DeTriangle* tri : Atriangles)
		if (!tri->isDegeneration())
			reserve.push_back(tri);
	Atriangles = reserve;
	return count;
}

void DeMesh::decimation(real percent, real threshold){
	initialize();
	int m = mDeTriangles.size(), need = int(m * percent);
	vector<DeVertex*> meshVertexs = mVexCloud.getData();
	for (DeTriangle* tri : mDeTriangles){
		Matrix44 quadMatrix(tri->getPlaneParam());
		DeVertex* vex[3];
		for (int i = 0; i < 3; ++ i){
			vex[i] = meshVertexs[tri->getVexID(i)];
			vex[i]->addAdjacent(tri);
			vex[i]->merge(quadMatrix);
		}
		for (int i = 0; i < 3; ++ i)
			vex[i]->addNeighbor(vex[(i+1)%3]);
	}
	for (DeVertex* vex : meshVertexs){
		vector<DeVertex* >& neighbor = vex->getNeighbor();
		mVexCloud.findInBall(neighbor, mVexCloud.root, vex->getPos(), threshold * threshold);
		for (DeVertex* near : neighbor)
			Q.push(VertexPair(vex, near));
	}
	while (!Q.empty() && m > need){
		VertexPair pair = Q.top();
		Q.pop();
		if (_checkValid(pair))
			m -= contraction(pair);
	}
	for (int i = 0; i < meshVertexs.size(); ++ i)
		if (!merged[i])
			mVertexs.push_back(meshVertexs[i]);
	for (int i = 0; i < m; ++ i){
		mTriangles.push_back(new Triangle(mVertexs, 0, 0, 0));
	}
}

}; // namespace Decimation
