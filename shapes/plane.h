// shapes/plane.h
#ifndef SHAPES_PLANE_H
#define SHAPES_PLANE_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"

namespace Raytracer {

class Plane : public Primitive{
public:
	Plane(Vec3f aPos, Vec3f aNorm) : mNorm(aNorm){
		mNorm.Normalize();
		mD = - dot(aPos, mNorm);
	}
	Plane(Vec3f aNorm, real aD) : mNorm(aNorm), mD(aD){
		mNorm.Normalize();
	}
	string getType(){
		return "Plane";
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	int intersect(const Ray& aRay, Intersection& isect);
	int intersectP(const Ray& aRay);
private:
	Vec3f mNorm;
	//Vec3f mUdir, mVdir;
	real mD;
};

}; // namespace Raytrace

#endif // SHAPES_PLANE_H
