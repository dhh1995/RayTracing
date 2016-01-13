#include "sphere.h"

namespace Raytracer {

void Sphere::setIsect(Intersection& isect, real dist, Vec3f pos){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	Vec3f norm = (pos - mCentre).Normalize();
	isect.setNorm(norm);

	// real x = norm.x, y = norm.y, z = norm.z;
	// real u,v;
	// if (abs(x) < EPS && abs(y) < EPS)
	// 	u = 0, v = 0;
	// else{
	// 	u = atan2(y, x) / 2 / PI;
	// 	v = asin(z) / 2 / PI;
	// }
	// isect.setUV(make_pair(u,v));
}

bool Sphere::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Sphere");
	Vec3f v = ray.o - mCentre;
	real b = -dot(v, ray.d);
	real det = (b * b) - dot(v, v) + mSqRadius;
	real dist = isect.getDist();
	bool back = false;
	if (det > 0){
		det = sqrtf(det);
		real i1 = b - det;
		real i2 = b + det;
		if (i2 > 0){
			if (i1 < 0){
				if (i2 < dist){
					dist = i2;
					setIsect(isect, dist, ray(dist));
					return HIT;
				}
			}else{
				if (i1 < dist){
					dist = i1;
					setIsect(isect, dist, ray(dist));
					return HIT;
				}
			}
		}
	}
	return MISS;
}

bool Sphere::intersectP(const Ray& aRay){
	//NotImplemented
	return false;
}

}; // namespace Raytracer
