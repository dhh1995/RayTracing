// core/primitive.h
#ifndef CORE_PRIMITIVE_H
#define CORE_PRIMITIVE_H

#include "common.h"
#include "shape.h"
#include "material.h"

namespace Raytracer {

class Primitive{
public:
	Primitive():mShape(NULL), mMaterial(NULL){
	}
	Shape* getShape(){
		return mShape;
	}
	void setShape(Shape* aShape){
		mShape = aShape;
	}
	Material* getMaterial(){
		return mMaterial;
	}
	void setMaterial(Material* aMaterial){
		mMaterial = aMaterial;
	}
	int intersect(const Ray& ray, Intersection& isect){
		return mShape->intersect(ray, isect);
	}
	int intersectP(const Ray& ray){
		return mShape->intersectP(ray);
	}
	void setName(char* aName);
	char* getName(){
		return mName;
	}
	Color getColor(Vec3f pos){
		if (mMaterial->haveTexture()){
			//pair<int, int> res = mShape->getCoordinate(pos);
			return mMaterial->getColor(pos.x, pos.z);
		}else
			return mMaterial->getColor();
	}
	Vec3f getNorm(Vec3f pos){
		return mShape->getNorm(pos);
	}
	// virtual ~Primitive(){
	// 	delete mShape;
	// 	delete mMaterial;
	// 	//delete mName;
	// }
private:
	Shape* mShape;
	Material* mMaterial;
	char* mName;
};

}; // namespace Raytrace

#endif // CORE_PRIMITIVE_H
