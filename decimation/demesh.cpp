#include "demesh.h"

namespace Decimation {

bool debug = false;
int useless;
int forbidCount;
const int RESERVE_NUMBER_TRIANGLE = 400;

int DeMesh::contraction(VertexPair P){
	int count = 0;
	DeVertex* A = P.A;
	DeVertex* B = P.B;
	if (A == B)
		return 0;

	//A->prt();
	//B->prt();
	//deal with fold over <- flip normal
	vector<DeTriangle*> Atri = A->getAdjacent();
	vector<DeTriangle*> Btri = B->getAdjacent();

	// for (DeTriangle* tri : Atri)
	// 	if (tri->isDegeneration()){
	// 		A->prt();
	// 		B->prt();
	// 		puts("!!");
	// 		return 10000000;
	// 	}

	// for (DeTriangle* tri : Btri)
	// 	if (tri->isDegeneration()){
	// 		A->prt();
	// 		B->prt();
	// 		puts("!!");
	// 		return 10000000;
	// 	}

	Vec3f Apos = A->getPos(), Bpos = B->getPos();
	A->setPos(P.mTarget);
	B->setPos(P.mTarget);
	bool forbid = false;
	for (DeTriangle* tri : Atri)
		if (!tri->isDegeneration() && !tri->haveVertex(B) && tri->flipNorm())
			forbid = true;
	for (DeTriangle* tri : Btri)
		if (!tri->isDegeneration() && !tri->haveVertex(A) && tri->flipNorm())
			forbid = true;
	//printf("%d\n",forbid);
	if (forbid){
		//printf("%d\n", ++ forbidCount);
		A->setPos(Apos);
		B->setPos(Bpos);
		return 0;
	}

	if (debug){
		A->prt();
		B->prt();
	}
	//printf("A = %d B = %d\n",A->getID(), B->getID());
	//A->setPos(P.mTarget);
	A->merge(B->getQuadMatrix());
	if (debug){
		A->getPos().prt();
		A->getQuadMatrix().prt();
	}
	timeStamp[A->getID()] = ++Time;
	timeStamp[B->getID()] = -1;
	vector<DeVertex* >& neighbors = A->getNeighbor();
	neighbors.erase(find(neighbors.begin(), neighbors.end(), B));
	for (DeVertex* vex : B->getNeighbor()){
		if (A != vex && find(neighbors.begin(), neighbors.end(), vex) == neighbors.end()){// Bruteforce
			neighbors.push_back(vex);
			vex->changeNeighbor(B, A);
		}
	}
	for (DeVertex* vex : neighbors) if (A != vex)
		Q.push(VertexPair(A, vex, Time));
	vector<DeTriangle*> & Atriangles = A->getAdjacent();
	vector<DeTriangle*> & Btriangles = B->getAdjacent();
	for (DeTriangle* tri : Btriangles)
		if (!tri->isDegeneration())
			if (tri->haveVertex(A)){
				tri->setDegeneration(), ++ count;
				//delete degenation triangle from vertex C.
				for (int i = 0; i < 3; ++ i){
					DeVertex* C = static_cast<DeVertex*> (tri->getVex(i));
					if (C != A && C != B){
						vector<DeTriangle*> & Ctri = C->getAdjacent();
						Ctri.erase(find(Ctri.begin(), Ctri.end(), tri));
					}
				}
			}else{
				tri->changeVertex(B, A);
				Atriangles.push_back(tri);
			}
	Btriangles.clear();
	vector<DeTriangle*> reserve;
	for (DeTriangle* tri : Atriangles)
		if (!tri->isDegeneration()){
			tri->updateNorm();
			reserve.push_back(tri);
		}
	Atriangles = reserve;

	// if (Atriangles.size() > 13){
	// if (neighbors.size() > 40){
	// 	printf("%d\n",Atriangles.size());
	// 	printf("%d\n",neighbors.size());
	// 	for (int i = 0; i < neighbors.size(); ++ i)
	// 		for (int j = 0; j < i; ++ j)
	// 			if (neighbors[i] == neighbors[j])
	// 				printf("%d %d\n",i,j);
	// }

	return count;
}

void DeMesh::decimation(int need, real threshold){
	//printf("!! need = %d\n",need);
	initialize(threshold > 0.0f);
	vector<DeVertex*> meshVertexs = mVexCloud.getData();
	for (DeTriangle* tri : mDeTriangles){
		Matrix44 quadMatrix(tri->getPlaneParam());
		//tri->prt();
		//quadMatrix.prt();
		DeVertex* vex[3];
		for (int i = 0; i < 3; ++ i){
			vex[i] = static_cast<DeVertex*>(tri->getVex(i));
			vex[i]->addAdjacent(tri);
			vex[i]->merge(quadMatrix);
		}
		for (int i = 0; i < 3; ++ i)
			vex[i]->addNeighbor(vex[(i+1)%3]);
	}
	for (DeVertex* vex : meshVertexs){
		//vex->prt();
		vector<DeVertex* >& neighbors = vex->getNeighbor();
		if (threshold > 0.0f){
			// int cnt = 0;
			// for (DeVertex* i : meshVertexs){
			// 	if ((i->getPos() - vex->getPos()).L2() < threshold * threshold)
			// 		++cnt;
			// }
			//int last = neighbors.size();
			mVexCloud.findInBall(neighbors, mVexCloud.root, vex->getPos(), threshold * threshold);
			// if (cnt != neighbors.size() - last)
			// 	printf("bf = %d kdtree = %d\n", cnt,  neighbors.size() - last);
		}
		for (DeVertex* near : neighbors) if (vex != near){
			//if (vex->getID() < near->getID()){
				VertexPair pair(vex, near, Time);
				// vex->prt();
				// near->prt();
				// printf("%lf\n", pair.mError);
				Q.push(pair);
			//}
		}
	}

	//printf("%d %d %d %d\n",mVertexs.size(), mTriangles.size(), meshVertexs.size(),  mDeTriangles.size());
	printf("%d\n",Q.size());
	int m = mDeTriangles.size(), last = m;
	while (!Q.empty() && m > need){
		VertexPair pair = Q.top();
		Q.pop();
		if (_checkValid(pair))
			m -= contraction(pair);
		if (last - m > 1000){
			//debug = true;
			last = m;
			printf("error = %.15lf\n",pair.mError);
			printf("Triangles rest 			%d 		heap size = %d\n", m, Q.size());
		}
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
				DeVertex* vex = static_cast<DeVertex*>(tri->getVex(i));
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

bool DeMesh::_checkValid(VertexPair P){
	int AID = P.A->getID(), BID = P.B->getID();
	if (debug){
		bool flag = false;
		if (timeStamp[AID] < 0)
			printf("vertex %d merged\n", AID), flag = true;
		if (timeStamp[BID] < 0)
			printf("vertex %d merged\n", BID), flag = true;
		if (timeStamp[AID] > P.mTimeStamp)
			printf("vertex %d out of date\n", AID), flag = true;
		if (timeStamp[BID] > P.mTimeStamp)
			printf("vertex %d out of date\n", BID), flag = true;
		if (flag)
			printf("%d\n", ++ useless);

	}
	if (timeStamp[AID] < 0 || timeStamp[AID] > P.mTimeStamp)
		return false;
	if (timeStamp[BID] < 0 || timeStamp[BID] > P.mTimeStamp)
		return false;
	return true;
}

}; // namespace Decimation
