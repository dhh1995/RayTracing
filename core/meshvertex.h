// core/meshvertex.h
#ifndef CORE_MESHVERTEX_H
#define CORE_MESHVERTEX_H

#include "common.h"
#include "vertex.h"
#include "shapes/triangle.h"

namespace Raytracer {

//for mesh decimation

class MeshVertex : public Vertex{
public:
	MeshVertex(const Vertex &A, int id) : Vertex(A), id(id){
		mQuad = Matrix44(0.0f);
	}
	void merge(Matrix44 aQuad){
		mQuad += aQuad;
	}
	real getAxis(int dim) const{
		return mPos[dim];
	}
	static real computeCost(MeshVertex* A, MeshVertex* B, Vec3f &target);
private:
	int id;
	Matrix44 mQuad;
	vector<Triangle* > mAdjecent;
};

struct VertexPair{
	VertexPair(MeshVertex* A, MeshVertex* B) : A(A), B(B){
		mError = MeshVertex::computeCost(A, B, mTarget);
	}
	bool operator < (const VertexPair &O) const{
		return mError > O.mError;
	}
	// bool operator > (const VertexPair &O) const{
	// 	return error < O.error;
	// }
	MeshVertex* A;
	MeshVertex* B;
	real mError;
	Vec3f mTarget;
};

}; // namespace Raytrace

#endif // CORE_MESHVERTEX_H
