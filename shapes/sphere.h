// shapes/sphere.h
#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"

namespace Raytracer {

class Sphere : public Primitive{
public:
	Sphere(Vec3f aCentre, real aRadius) : mCentre(aCentre), mRadius(aRadius){
		mSqRadius = mRadius * mRadius;
	}
	string getType(){
		return "Sphere";
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool intersect(const Ray& aRay, Intersection& isect);
	bool intersectP(const Ray& aRay);
private:
	Vec3f mCentre;
	real mRadius;
	real mSqRadius;
};

}; // namespace Raytracer

#endif // SHAPES_SPHERE_H
