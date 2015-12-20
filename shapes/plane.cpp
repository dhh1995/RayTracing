#include "plane.h"

namespace Raytracer {

int Plane::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Plane");
	float d = dot(mNorm, ray.d);
	if (d != 0)
	{
		float dist = -(dot(mNorm, ray.o) + mD) / d;
		if (dist > 0)
		{
			if (dist < isect.getDist()) 
			{
				isect.setDist(dist);
				isect.setPos(ray(dist));
				return HIT;
			}
		}
	}
	return MISS;
}

int Plane::intersectP(const Ray& aRay){

}

}; // namespace Raytrace
