#include "box.h"

namespace Raytracer {

void Box::getNearFar(const Ray& ray, real& near, real& far){
	Vec3f rMin = mMin - ray.o;
	Vec3f rMax = mMax - ray.o;
	rMin /= ray.d;
	rMax /= ray.d;
	for (int i = 0; i < 3; ++ i)
		if (rMin[i] > rMax[i])
			swap(rMin[i], rMax[i]);
	near = rMin.getMax();
	far  = rMax.getMin();
	// if (near < far){
	// 	ray.prt();
	// 	printf("%lf %lf\n", near, far);
	// }
}

void Box::setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	isect.setBack(backSide);
	Vec3f norm = getNorm(pos);
	//pos.prt();
	//norm.prt();
	isect.setNorm(norm);
	isect.setColor(mMaterial->getColor());
}

bool Box::intersect(const Ray& ray, Intersection& isect){
	real near = -1., far = -1.;
	getNearFar(ray, near, far);
	if (near > far || far < 0)
		return MISS;
	real dist = INF;
	bool backSide = false;
	if (near < 0){
		dist = far;
		backSide = true;
	}else
		dist = near;
	if (dist < isect.getDist())
		setIsect(isect, dist, ray(dist), backSide);
	return HIT;
}

bool Box::intersectP(const Ray& aRay){
	
}

}; // namespace Raytrace
