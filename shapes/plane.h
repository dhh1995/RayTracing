// shapes/plane.h
#ifndef SHAPES_PLANE_H
#define SHAPES_PLANE_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"

namespace Raytracer {

class Plane : public Primitive{
public:
	Plane(Vec3f aNorm, Vec3f aU, Vec3f aPos) : mNorm(aNorm), mU(aU){
		mNorm.Normalize();
		mU.Normalize();
		mV = cross(mNorm, mU);
		mD = - dot(aPos, mNorm);
	}
	Plane(Vec3f aNorm, Vec3f aU, real aD) : mNorm(aNorm), mU(aU), mD(aD){
		mNorm.Normalize();
		mU.Normalize();
		mV = cross(mNorm, mU);
	}
	string getType(){
		return "Plane";
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool intersect(const Ray& aRay, Intersection& isect);
	bool intersectP(const Ray& aRay);
private:
	//Norm = U x V;
	Vec3f mNorm;
	Vec3f mU, mV;
	real mD;
};

}; // namespace Raytrace

#endif // SHAPES_PLANE_H
