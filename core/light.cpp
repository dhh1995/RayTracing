#include "light.h"

namespace Raytracer {

bool AreaLight::intersect(const Ray& ray, Intersection& isect){
	float d = dot(mNorm, ray.d);
	if (d != 0){
		float dist = -(dot(mNorm, ray.o) + mD) / d;
		if (dist > 0){
			if (dist < isect.getDist()){
				isect.setDist(dist);
				isect.setLight(true);
				return HIT;
			}
		}
	}
	return MISS;
}

}; // namespace Raytracer