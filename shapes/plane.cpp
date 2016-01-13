#include "plane.h"

namespace Raytracer {

void Plane::setIsect(Intersection& isect, real dist, Vec3f pos){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	Vec3f norm = mNorm;
	isect.setNorm(norm);
	isect.setUV(make_pair(dot(pos, mU), dot(pos, mV)));
}

bool Plane::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Plane");
	float d = dot(mNorm, ray.d);
	if (d != 0){
		float dist = -(dot(mNorm, ray.o) + mD) / d;
		if (dist > 0){
			if (dist < isect.getDist()){
				setIsect(isect, dist, ray(dist));
				return HIT;
			}
		}
	}
	return MISS;
}

bool Plane::intersectP(const Ray& aRay){

}

}; // namespace Raytracer
