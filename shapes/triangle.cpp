#include "triangle.h"

namespace Raytracer {

void Triangle::setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	isect.setBack(backSide);
	//printf("%d\n",backSide);
	Vec3f norm = mNorm;
	if (mMaterial->haveTexture()){
		//TODO
	}else{
		isect.setNorm(norm);
		isect.setColor(mMaterial->getColor());
	}
}

// int Triangle::intersect(const Ray& ray, Intersection& isect){
// 	Vec3f Rd = ray.d;
// 	Vec3f S = A - ray.o;
// 	real res = det(Rd, E1, E2);
// 	if (abs(res) < EPS)
// 		return MISS;
// 	Vec3f ans = Vec3f(det(S, E1, E2), det(Rd, S, E2), det(Rd, E1, S)) / res;
// 	if (ans.x < 0 || ans.y < 0 ||  ans.z < 0 || ans.y + ans.z > 1)
// 		return MISS;
// 	isect.setDist(ans.x);
// 	isect.setPos(ray(ans.x));
// 	return HIT;
// }

bool Triangle::inside(const Vec3f pos){
	return dot(cross(B->getPos() - A->getPos(), pos - A->getPos()), mNorm) >= 0
		&& dot(cross(C->getPos() - B->getPos(), pos - B->getPos()), mNorm) >= 0
		&& dot(cross(A->getPos() - C->getPos(), pos - C->getPos()), mNorm) >= 0;
}

bool Triangle::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Triangle");
	float d = dot(mNorm, ray.d);
	if (d != 0){
		float dist = -(dot(mNorm, ray.o) + mD) / d;
		if (dist > 0){
			if (dist < isect.getDist()) {
				Vec3f pos = ray(dist);
				if (!inside(pos))
					return MISS;
				setIsect(isect, dist, pos, dot(ray.d, mNorm) > 0);
				return HIT;
			}
		}
	}
	return MISS;
}

bool Triangle::intersectP(const Ray& ray){

}


}; // namespace Raytrace
