#include "demesh.h"

namespace Decimation {

bool debug = false;

int DeMesh::contraction(VertexPair P){
	int count = 0;
	DeVertex* A = P.A;
	DeVertex* B = P.B;
	//printf("A = %d B = %d\n",A->getID(), B->getID());
	A->setPos(P.mTarget);
	A->merge(B->getQuadMatrix());
	timeStamp[A->getID()] = ++Time;
	timeStamp[B->getID()] = -1;
	vector<DeVertex* >& neighbor = A->getNeighbor();
	//neighbor.erase(find(neighbor.begin(), neighbor.end(), B));
	for (DeVertex* vex : B->getNeighbor())
		if (A != vex)
			neighbor.push_back(vex);
	//for (DeVertex* vex : neighbor)
	//	Q.push(VertexPair(A, vex, Time));

	vector<DeTriangle*> & Atriangles = A->getAdjacent();
	vector<DeTriangle*> & Btriangles = B->getAdjacent();
	for (DeTriangle* tri : Btriangles)
		if (!tri->isDegeneration())
			if (tri->haveVertex(A)){
				//printf("tri = %d\n", tri);
				tri->setDegeneration(), ++ count;
			}else{
				tri->changeVertex(B, A);
				Atriangles.push_back(tri);
			}
	Btriangles.clear();
	vector<DeTriangle*> reserve;
	for (DeTriangle* tri : Atriangles)
		if (!tri->isDegeneration())
			reserve.push_back(tri);
	Atriangles = reserve;
	return count;
}

void DeMesh::decimation(real percent, real threshold){
	initialize(threshold > 0.0f);
	int m = mDeTriangles.size(), need = int(m * percent);
	vector<DeVertex*> meshVertexs = mVexCloud.getData();
	for (DeTriangle* tri : mDeTriangles){
		Matrix44 quadMatrix(tri->getPlaneParam());
		DeVertex* vex[3];
		for (int i = 0; i < 3; ++ i){
			vex[i] = dynamic_cast<DeVertex*>(tri->getVex(i));
			vex[i]->addAdjacent(tri);
			vex[i]->merge(quadMatrix);
		}
		for (int i = 0; i < 3; ++ i)
			vex[i]->addNeighbor(vex[(i+1)%3]);
	}
	for (DeVertex* vex : meshVertexs){
		vector<DeVertex* >& neighbor = vex->getNeighbor();
		if (threshold > 0.0f)
			mVexCloud.findInBall(neighbor, mVexCloud.root, vex->getPos(), threshold * threshold);
		for (DeVertex* near : neighbor)
			if (vex->getID() < near->getID())
				Q.push(VertexPair(vex, near, Time));
	}

	//printf("%d %d %d %d\n",mVertexs.size(), mTriangles.size(), meshVertexs.size(),  mDeTriangles.size());
	printf("%d\n",Q.size());

	while (!Q.empty() && m > need){
		VertexPair pair = Q.top();
		Q.pop();
		if (_checkValid(pair))
			m -= contraction(pair);
		if (debug)
			printf("%lf\n",pair.mError);
		if (m % 100 == 0)
			printf("%d\n", m);
	}

	printf("%d %d\n",m, Q.size());

	mVertexs.clear();
	int n = 0;
	for (DeVertex* vex : meshVertexs){
		if (timeStamp[vex->getID()] >= 0){
			vex->setID(n ++);
			mVertexs.push_back(vex);
		}else
			delete vex;
	}
	mTriangles.clear();
	for (DeTriangle* tri : mDeTriangles){
		if (!tri->isDegeneration()){
			int id[3];	//recover IDs for Triangle
			for (int i = 0; i <  3; ++ i){
				DeVertex* vex = dynamic_cast<DeVertex*>(tri->getVex(i));
				id[i] = vex->getID();
			}
			tri->setVexIDs(id);
			mTriangles.push_back(tri);
		}else
			delete tri;
	}
	mDeTriangles.clear();
	printf("%d\n",mVertexs.size());
	printf("%d\n",mTriangles.size());
}

}; // namespace Decimation
