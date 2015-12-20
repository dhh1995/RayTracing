// core/intersection.h
#ifndef CORE_INTERSECTION_H
#define CORE_INTERSECTION_H

#include "common.h"
#include "primitive.h"

namespace Raytracer {

class Intersection{
public:
	Intersection(Primitive* aPrim = NULL, Vec3f aPos = ORIGINAL, real aDist = INF) : mPrim(aPrim), mPos(aPos), mDist(aDist){
	}
	void setPos(Vec3f aPos){
		mPos = aPos;
	}
	Vec3f getPos() const{
		return mPos;
	}
	void setDist(real aDist){
		mDist = aDist;
	}
	real getDist()  const{
		return mDist;
	}
	void setPrim(Primitive* aPrim){
		mPrim = aPrim;
	}
	Primitive* getPrim() const{
		return mPrim;
	}
	Color getColor(){
	}
private:
	real mDist;
	Vec3f mPos;
	Primitive* mPrim;
};

}; // namespace Raytracer

#endif // CORE_INTERSECTION_H
