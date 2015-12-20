// shapes/plane.h
#ifndef SHAPES_PLANE_H
#define SHAPES_PLANE_H

#include "core/common.h"
#include "core/shape.h"
#include "core/intersection.h"

namespace Raytracer {

class Plane : public Shape{
public:
	Plane(Vec3f aPos, Vec3f aNorm) : mNorm(aNorm){
		mNorm.Normalize();
		mD = dot(aPos, mNorm); 
	}
	Plane(Vec3f aNorm, real aD) : mNorm(aNorm), mD(aD){
		mNorm.Normalize();
	}
	string getType(){
		return "Plane";
	}
	int intersect(const Ray& aRay, Intersection& isect);
	int intersectP(const Ray& aRay);
	Vec3f getNorm(Vec3f pos){
		return mNorm;
	}
private:
	Vec3f mNorm;
	real mD;
};

}; // namespace Raytrace

#endif // SHAPES_PLANE_H
