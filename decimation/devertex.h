// core/devertex.h
#ifndef DECIMATION_DEVERTEX_H
#define DECIMATION_DEVERTEX_H

#include "core/common.h"
#include "core/vertex.h"
#include "decimation/detriangle.h"
#include "shapes/triangle.h"

using namespace Raytracer;

namespace Decimation {

class DeVertex : public Vertex{
public:
	DeVertex(const Vertex A, int id) : Vertex(A), id(id){
		mQuad = Matrix44();
	}
	Matrix44 getQuadMatrix(){
		return mQuad;
	}
	void merge(Matrix44 aQuad){
		mQuad += aQuad;
	}
	void setID(int aID){
		id = aID;
	}
	int getID(){
		return id;
	}
	real getAxis(int dim) const{
		return mPos[dim];
	}
	vector<DeVertex* >& getNeighbor(){
		return mNeighbor;
	}
	vector<DeTriangle* >& getAdjacent(){
		return mAdjecent;
	}
	void addNeighbor(DeVertex* vex){
		mNeighbor.push_back(vex);
	}
	void addAdjacent(DeTriangle* tri){
		mAdjecent.push_back(tri);
	}
	void prt(){
		printf("id = %d\npos = ", id);
		mPos.prt();
		mQuad.prt();
	}
	static real computeCost(DeVertex* A, DeVertex* B, Vec3f &target);
private:
	int id;
	Matrix44 mQuad;
	vector<DeVertex* > mNeighbor;
	vector<DeTriangle* > mAdjecent;
};

struct VertexPair{
	VertexPair(DeVertex* A, DeVertex* B, int time) : A(A), B(B), mTimeStamp(time){
		mError = DeVertex::computeCost(A, B, mTarget);
	}
	bool operator < (const VertexPair &O) const{
		return mError > O.mError;
	}
	DeVertex* A;
	DeVertex* B;
	real mError;
	Vec3f mTarget;
	int mTimeStamp;
};

}; // namespace Decimation

#endif // DECIMATION_DEVERTEX_H
