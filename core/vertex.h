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
	void setPos(Vec3f aPos){
		mPos = aPos;
	}
	Vec3f getNorm(){
		return mNorm;
	}
	void prt(FILE* out){
		fprintf(out, "v %lf %lf %lf\n", mPos.x, mPos.y, mPos.z);
	}
	virtual void prt(){
		printf("pos = ");
		mPos.prt();
		// printf("norm = ");
		// mNorm.prt();
	}
protected:
	Vec3f mPos;
	Vec3f mNorm;
};

}; // namespace Raytracer

#endif // CORE_VERTEX_H
