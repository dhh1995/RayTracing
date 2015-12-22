#include "plane.h"

namespace Raytracer {

void Plane::setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	Vec3f norm = mNorm;
	if (backSide)
		norm = -norm;
	if (mMaterial->haveTexture()){
		real u = pos.x, v = pos.y;
		isect.setNorm(norm + mMaterial->getNorm(u, v));
		isect.setColor(mMaterial->getColor(u, v));
	}else{
		isect.setNorm(norm);
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
				setIsect(isect, dist, ray(dist), dot(ray.d, mNorm) > 0);
				return HIT;
			}
		}
	}
	return MISS;
}

int Plane::intersectP(const Ray& aRay){

}

}; // namespace Raytrace
