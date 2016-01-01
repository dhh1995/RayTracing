// core/devertex.h
#ifndef CORE_DEVERTEX_H
#define CORE_DEVERTEX_H

#include "core/common.h"
#include "core/vertex.h"
#include "shapes/triangle.h"

using namespace RayTracer;

namespace Decimation {

class DeVertex : public Vertex{
public:
	DeVertex(const Vertex &A, int id) : Vertex(A), id(id){
		mQuad = Matrix44(0.0f);
	}
	void merge(Matrix44 aQuad){
		mQuad += aQuad;
	}
	real getAxis(int dim) const{
		return mPos[dim];
	}
	static real computeCost(DeVertex* A, DeVertex* B, Vec3f &target);
private:
	int id;
	Matrix44 mQuad;
	vector<Triangle* > mAdjecent;
};

struct VertexPair{
	VertexPair(DeVertex* A, DeVertex* B) : A(A), B(B){
		mError = DeVertex::computeCost(A, B, mTarget);
	}
	bool operator < (const VertexPair &O) const{
		return mError > O.mError;
	}
	// bool operator > (const VertexPair &O) const{
	// 	return error < O.error;
	// }
	DeVertex* A;
	DeVertex* B;
	real mError;
	Vec3f mTarget;
};

}; // namespace Decimation

#endif // CORE_DEVERTEX_H
