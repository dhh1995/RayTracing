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
	MeshVertex(const Vertex A) : Vertex(A){
		
	}

	real getAxis(int dim) const{
		return mPos[dim];
	}
private:
	Matrix44* mQuad;
	vector<Triangle* > mAdjecent;
};

struct VertexPair{
	MeshVertex* A;
	MeshVertex* B;
	real error;
	bool operator < (const VertexPair &O) const{
		return error > O.error;
	}
	// bool operator > (const VertexPair &O) const{
	// 	return error < O.error;
	// }
};

}; // namespace Raytrace

#endif // CORE_MESHVERTEX_H
