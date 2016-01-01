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
	Vec3f getNorm(){
		return mNorm;
	}
	void prt(){
		printf("pos = ");
		mPos.prt();
		printf("norm = ");
		mNorm.prt();
	}
protected:
	Vec3f mPos;
	Vec3f mNorm;
};

}; // namespace Raytrace

#endif // CORE_VERTEX_H
