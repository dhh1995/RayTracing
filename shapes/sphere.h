// shapes/sphere.h
#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

#include "core/common.h"
#include "core/shape.h"
#include "core/intersection.h"

namespace Raytracer {

class Sphere : public Shape{
public:
	Sphere(Vec3f aCentre, real aRadius) : mCentre(aCentre), mRadius(aRadius){
		mSqRadius = mRadius * mRadius;
	}
	string getType(){
		return "Sphere";
	}
	int intersect(const Ray& aRay, Intersection& isect);
	int intersectP(const Ray& aRay);
	Vec3f getNorm(Vec3f pos){
		return (pos - mCentre).Normalize();
	}
private:
	Vec3f mCentre;
	real mRadius;
	real mSqRadius;
};

}; // namespace Raytrace

#endif // SHAPES_SPHERE_H
