// core/vertex.h
#ifndef CORE_VERTEX_H
#define CORE_VERTEX_H

#include "common.h"

namespace Raytracer {

class Vertex{
public:
	Vertex(Vec3f aPos):mPos(aPos){
	}
	Vec3f getPos(){
		return mPos;
	}
	real getAxis(int dim) const{
		return mPos[dim];
	}
	void prt(){
		printf("pos = ");
		mPos.prt();
		printf("norm = ");
		mNorm.prt();
	}
private:
	Vec3f mPos; // pos
	Vec3f mNorm; // norm
	Matrix44* mQuad;
	//vector<Triangle* > mAdjecent;
};

struct VertexPair{
	Vertex* A;
	Vertex* B;
	real error;
	bool operator < (const VertexPair &O) const{
		return error > O.error;
	}
	// bool operator > (const VertexPair &O) const{
	// 	return error < O.error;
	// }
};

}; // namespace Raytrace

#endif // CORE_VERTEX_H
