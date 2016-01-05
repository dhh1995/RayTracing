#include "photonrenderer.h"

namespace Raytracer {

void PhotonRenderer::photonTracing(PhotonMap &pm, Photon* photon){
	Intersection isect;
	Ray ray = photon->getRay();
	mPhotonEmits++;
	if (mScene->intersect(ray, isect) == MISS)
		return;
	Material* matter = isect.getPrim()->getMaterial();
	Vec3f pi = isect.getPos();
	Vec3f norm = isect.getNorm();
	Color color = isect.getColor();

	real diff = matter->getDiffuse();
	real spec = matter->getSpecular();
	// real refr = matter->getRefraction();
	// real refl = matter->getReflection();

	real ran = Sampler::getRand();
	if (ran < spec){
		//spec, reflection or refraction
		real rindex = matter->getRefrIndex();
		real n = 1.0f / rindex;
		Vec3f N = norm;
		if (isect.isBack()){
			n = rindex;
			N = -N;
		}
		real cosI = -dot(N, ray.d);
		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
		real refl = 1;
		Vec3f R;
		if (cosT2 > 0.0f){
			// need to adopt fresnel equation
			// https://en.wikipedia.org/wiki/Fresnel_equations
			// http://www.cnblogs.com/starfallen/p/4019350.html
			// ? refl = 1 - refr;
			real cosT = sqrt(cosT2);
			real r_p = (n * cosI - cosT) / (n * cosI + cosT);
			real r_v = (cosI - n * cosT) / (cosI + n * cosT);
			refl = (r_p * r_p + r_v * r_v) / 2.0;
			// refr = 1 - refl;
		}// else totally internal reflection
		if (ran < refl * spec)
			R = ray.d - 2.0f * dot(ray.d, N) * N;
		else
			R = (n * ray.d) + (n * cosI - sqrt( cosT2 )) * N;
		photon->setRay(Ray(pi + EPS * R, R));
		photonTracing(pm, photon);
	}else if (ran < diff + spec){
		// diffuse, get new direction and save it in photon map
		pm.add(photon);
		Vec3f dir = _getDiffuseDir(norm);
		photon->setRay(Ray(pi + EPS * dir, dir));
		photonTracing(pm, photon);
	}else
		pm.add(photon);
}

void PhotonRenderer::genGlobalPhotonMap(string path, int require){
	int m = mScene->getLights().size();
	require /= m;
	for (Light* light : mScene->getLights()){
		if (light->getType() == "point"){
			int emits = 0;
			Vec3f centre = light->getPos();
			while (emits < require){
				Vec3f dir = _getRandomDir();
				Photon* photon = new Photon(Ray(centre, dir), Vec3f(1, 1, 1) / real(require));
				photonTracing(mGlobal, photon);
			}
		}
	}
}

void PhotonRenderer::genCausticPhotonMap(string path, int require){

}

void PhotonRenderer::render(const Args& args){
	genGlobalPhotonMap("global", 1000);
	genCausticPhotonMap("caustic", 100);

	//assert(mCamera != NULL);
	vector<Ray> rays = mCamera->generateRays();
	int nRays = rays.size(), cnt = 0, tot = 10;
	int lastShow = -1;
	cout << "number of pixels " << nRays << endl;
	for (Ray ray : rays){
		++cnt;
		int percent = cnt * tot / nRays;
		if (percent != lastShow){
			lastShow = percent;
			printf("%d/%d\n", percent,tot);
		}

		int x = ray.mFilmX, y = ray.mFilmY;
		Color res;
		real dist;
		rayTracing(ray, res, 0, 1, dist);
		mCamera->getFilm()->setColor(x, y, res);
	}
}

}; // namespace Raytracer
