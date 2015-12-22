#include "scene.h"

namespace Raytracer {

int Scene::intersect(const Ray& ray, Intersection& isect){
	//return mAggregate->intersect(aRay, aDist);
	isect.setDist(INF);
	int ans = MISS;
	for (Primitive* obj : mAggregate){
		Intersection tmp;
		//printf("%d\n", obj);
		int retval = obj->intersect(ray, tmp);
		if (retval != 0){
			if (tmp.getDist() < isect.getDist())
				isect = tmp;
			ans = retval;
		}
		//printf("%lf\n",tmp.getDist());
	}
	return ans;
}

int Scene::intersectP(const Ray& ray){
	
}

real Scene::calcShade(Light* light, Vec3f pos, Vec3f& dir){
	Vec3f center = light->getPos();
	dir = (center - pos).Normalize();
	Intersection isect;
	if (intersect(Ray(center, -dir), isect) != 0)
		if (isect.getPos() != pos)
			return 0.;
	return 1.;
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