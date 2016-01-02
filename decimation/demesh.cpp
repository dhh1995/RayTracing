#include "demesh.h"

namespace Decimation {

void DeMesh::contraction(VertexPair P){
	DeVertex* A = P.A;
	DeVertex* B = P.B;
	A->setPos(P.mTarget);
	vector<DeTriangle*> & Atriangles = A->getAdjacent();
	vector<DeTriangle*> & Btriangles = B->getAdjacent();

	//filter(Atriangles);
}

void DeMesh::decimation(real percent, real threshold){
	int m = mDeTriangles.size(), need = int(m * percent);
	vector<DeVertex*> meshVertexs = mVexCloud.getData();
	mVexCloud.construct();
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
			contraction(pair);
	}
}

bool DeMesh::_checkValid(VertexPair P){

}

}; // namespace Decimation
