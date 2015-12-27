#include "scene.h"

namespace Raytracer {

bool Scene::intersect(const Ray& ray, Intersection& isect){
	isect.setDist(INF);
	bool ans = mAggregate.intersect(ray, isect);
	for (Primitive* obj : mPrimitives){
		if (obj->intersect(ray, isect) == HIT)
			ans = HIT;
	}
	return ans;
}

bool Scene::intersectP(const Ray& ray){
	// int ans = mAggregate->intersectP(ray);
	// for (Primitive* obj : mPrimitives)
	// 	if (obj->intersectP(ray) != 0)
	// 		return 1;
	return MISS;
}

real Scene::calcShade(Light* light, Vec3f pos, Vec3f& dir){
	Vec3f center = light->getPos();
	dir = center - pos;
	if (light->getType() == "Point"){
		real dist = dir.length();
		dir /= dist;
		//printf("%lf\n",dist);
		Intersection isect;
		if (intersect(Ray(center, -dir), isect) == HIT){
			// center.prt();
			// pos.prt();
			// printf("%lf\n",isect.getDist());
			// isect.getPos().prt();
			if (isect.getDist() + EPS < dist)
				return 0.;
		}
		return 1.;
	}

	real res = 1;
	int n = 3, n2 = n / 2;
	for (int i=0;i<n;++i)
		for (int j=0;j<n;++j){
			Vec3f newCenter = light->getPos((i-n2) / 2. , (j-n2) / 2.);
			Vec3f d = newCenter - pos;
			//light->getPos((i-n2) / 2. , (j-n2) / 2.).prt();
			real dist = d.length();
			d /= dist;
			Intersection isect;
			if (intersect(Ray(newCenter, -d), isect) == HIT)
				if (isect.getDist() + EPS < dist)
					res -= 1. / n / n;
		}
	dir.Normalize();
	return res;
}

Color Scene::getLi(const Ray& ray, const Intersection& isect){
	Vec3f pi = isect.getPos();
	Vec3f N = isect.getNorm();
	Color color = isect.getColor();
	Material* matter = isect.getPrim()->getMaterial();

	Color res = mAmbient * matter->getKa();
	for (Light* light : mLights){
		Vec3f L;
		real shade = calcShade(light, pi, L);
		if (shade > 0){
			// calculate diffuse shading
			real diff = matter->getDiffuse();
			if (diff > 0){
				real dt = dot(L, N);
				if (dt > 0){
					real diffuse = dt * diff * shade;
					// add diffuse component to ray color
					res += diffuse * color * light->getColor();
				}
			}
			// determine specular component using Schlick's BRDF approximation
			real spec = matter->getSpecular();
			if (spec > 0){
				// point light source: sample once for specular highlight
				Vec3f R = L - 2.0f * dot( L, N ) * N;
				real dt = dot(ray.d, R);	
				if (dt > 0){
					real specular = dt * spec * shade / (50 - 50 * dt + dt);
					// add specular component to ray color
					res += specular * light->getColor();
				}
			}
		}
	}
	return res;
}

}; // namespace Raytrace