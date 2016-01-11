#include "scene.h"

namespace Raytracer {

bool Scene::intersect(const Ray& ray, Intersection& isect, bool ignoreLight){
	isect.setDist(INF);
	bool ans = mAggregate.intersect(ray, isect);
	for (Primitive* obj : mPrimitives)
		if (obj->intersect(ray, isect) == HIT)
			ans = HIT;

	if (ignoreLight)
		for (Light* obj : mLights)
			if (obj->intersect(ray, isect) == HIT)
				ans = HIT;
	return ans;
}

bool Scene::intersectP(const Ray& ray){
	// int ans = mAggregate->intersectP(ray);
	// for (Primitive* obj : mPrimitives)
	// 	if (obj->intersectP(ray) != 0)
	// 		return HIT;
	return MISS;
}

real Scene::visible(const Ray& ray, real dist){
	Intersection isect;
	if (intersect(ray, isect, true) == HIT)
		if (isect.getDist() + EPS < dist)
			return 0.0f;
	return 1.0f;
}

}; // namespace Raytracer