#include "plane.h"

namespace Raytracer {

void Plane::setIsect(Intersection& isect, real dist, Vec3f pos){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	if (mMaterial->haveTexture()){
		real u = pos.x, v = pos.y;
		isect.setNorm(mNorm + mMaterial->getNorm(u, v));
		isect.setColor(mMaterial->getColor(u, v));
	}else{
		isect.setNorm(mNorm);
		isect.setColor(mMaterial->getColor());
	}
}

int Plane::intersect(const Ray& ray, Intersection& isect){
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

int Plane::intersectP(const Ray& aRay){

}

}; // namespace Raytrace
