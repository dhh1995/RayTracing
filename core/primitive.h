// core/primitive.h
#ifndef CORE_PRIMITIVE_H
#define CORE_PRIMITIVE_H

#include "common.h"
#include "material.h"

namespace Raytracer {

class Primitive{
public:
	Primitive():mMaterial(NULL){
	}
	void setName(char* aName){
		delete mName;
		mName = new char[strlen(aName) + 1]; 
		strcpy(mName, aName);
	}
	char* getName(){
		return mName;
	}
	Material* getMaterial(){
		return mMaterial;
	}
	void setMaterial(Material* aMaterial){
		mMaterial = aMaterial;
	}

	virtual string getType() = 0;
	virtual bool intersect(const Ray& ray, Intersection& isect) = 0;
	virtual bool intersectP(const Ray& ray) = 0;
	//virtual Vec3f getNorm(Vec3f pos) = 0;
	virtual Color getColor(Vec3f pos){
		mMaterial->getColor();
	}
protected:
	Material* mMaterial;
	char* mName;
};

}; // namespace Raytracer

#endif // CORE_PRIMITIVE_H
