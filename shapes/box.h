// shapes/box.h
#ifndef SHAPES_BOX_H
#define SHAPES_BOX_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"

namespace Raytracer {

class Box : public Primitive{
public:
	Box(){
		mMin = Vec3f(INF, INF, INF);
		mMax = -mMin;
	}
	void init(const Vec3f &A){
		mMin = mMax = A;
	}
	int argMaxDiff(){
		return (mMax - mMin).argMax();
	}
	void update(const Vec3f &A){
		minimize(mMin, A);
		maximize(mMax, A);
	}
	void update(const Box &A){
		minimize(mMin, A.mMin);
		maximize(mMax, A.mMax);
	}
	real minDist(Vec3f pos){
		//TODO
	}
	string getType(){
		return "Box";
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	int intersect(const Ray& aRay, Intersection& isect);
	int intersectP(const Ray& aRay);
private:
	Vec3f mMin, mMax;
};

}; // namespace Raytrace

#endif // SHAPES_BOX_H
