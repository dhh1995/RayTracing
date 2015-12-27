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
	Box(Vec3f A):mMin(A), mMax(A){
	}
	Box(Vec3f A, Vec3f B){
		mMin = A;
		mMax = A;
		update(B);
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
	real getMid(int dim){
		return (mMax[dim] + mMin[dim]) / 2.0f;
	}
	real minDist(Vec3f pos){
		//TODO
	}
	Vec3f getNorm(Vec3f pos){
		if (pos[0] < mMin[0] + EPS)
			return Vec3f(-1, 0, 0);
		if (pos[0] > mMax[0] - EPS)
			return Vec3f(1, 0, 0);
		if (pos[1] < mMin[1] + EPS)
			return Vec3f(0, -1, 0);
		if (pos[1] > mMax[1] - EPS)
			return Vec3f(0, 1, 0);
		if (pos[2] < mMin[2] + EPS)
			return Vec3f(0, 0, -1);
		if (pos[2] > mMax[2] - EPS)
			return Vec3f(0, 0, 1);
	}
	void getNearFar(const Ray& ray, real& near, real& far);
	string getType(){
		return "Box";
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool intersect(const Ray& aRay, Intersection& isect);
	bool intersectP(const Ray& aRay);
private:
	Vec3f mMin, mMax;
};

}; // namespace Raytrace

#endif // SHAPES_BOX_H
