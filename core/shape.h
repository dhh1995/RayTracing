// core/shape.h
#ifndef CORE_SHAPE_H
#define CORE_SHAPE_H

#include "common.h"
#include "transform.h"

namespace Raytracer {

class Shape{
public:
	Shape(){
		mShapeID = ++lastShapeID;
	}
	//Shape(Transform *o2w, Transform *w2o);
	int getID(){
		return mShapeID;
	}
	virtual string getType() = 0;
	virtual int intersect(const Ray& aRay, Intersection& isect) = 0;
	virtual int intersectP(const Ray& aRay) = 0;
	virtual Vec3f getNorm(Vec3f pos) = 0;
private:
	int mShapeID;
	static int lastShapeID;
	//Transform *objectToWorld, *worldToObject;
};

}; // namespace Raytrace

#endif // CORE_SHAPE_H
