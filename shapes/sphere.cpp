#include "sphere.h"

namespace Raytracer {

int Sphere::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Sphere");
	//ray.o.print();
	//mCentre.print();
	Vec3f v = ray.o - mCentre;
	//v.print();
	//ray.d.print();
	real b = -dot(v, ray.d);
	real det = (b * b) - dot(v, v) + mSqRadius;
	int retval = MISS;
	real dist = isect.getDist();
	//printf("%lf det = %lf\n", b, det);
	if (det > 0){
		det = sqrtf(det);
		real i1 = b - det;
		real i2 = b + det;
		if (i2 > 0){
			if (i1 < 0){
				if (i2 < dist){
					dist = i2;
					retval = INPRIM;
				}
			}else{
				if (i1 < dist){
					dist = i1;
					retval = HIT;
				}
			}
		}
	}
	//printf("retval = %d dist = %lf\n", retval, dist);
	isect.setDist(dist);
	isect.setPos(ray(dist));
	return retval;
}

int Sphere::intersectP(const Ray& aRay){
	//NotImplemented
	return false;
}

}; // namespace Raytrace
