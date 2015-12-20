// core/scene.h
#ifndef CORE_SCENE_H
#define CORE_SCENE_H

#include "common.h"
#include "light.h"
#include "primitive.h"
#include "intersection.h"

namespace Raytracer {

class Scene{
public:
	Scene(){
		mAggregate.clear();
		mLights.clear();
	}
	int intersect(const Ray& ray, Intersection& isect);
	int intersectP(const Ray& ray);
	void addLight(Light *aLight){
		mLights.push_back(aLight);
	}
	void addObject(Primitive *aObject){
		mAggregate.push_back(aObject);
	}
	real calcShade(Light* light, Vec3f pos, Vec3f& dir);
	Color getLi(const Ray& ray, const Intersection& isect);
	//void loadObj();
private:
	//Primitive* mAggregate;
	vector<Primitive* > mAggregate;
	vector<Light* > mLights;
};

}; // namespace Raytrace

#endif // CORE_SCENE_H
