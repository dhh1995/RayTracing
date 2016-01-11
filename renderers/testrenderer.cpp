#include "testrenderer.h"

bool debug = false;

namespace Raytracer {

real TestRenderer::_FresnelReflection(real n, real cosI, real cosT2){
	if (cosT2 > 0.0f){
		// need to adopt fresnel equation
		// https://en.wikipedia.org/wiki/Fresnel_equations
		// http://www.cnblogs.com/starfallen/p/4019350.html
		// ? refl = 1 - refr;
		real cosT = sqrt(cosT2);
		real r_p = (n * cosI - cosT) / (n * cosI + cosT);
		real r_v = (cosI - n * cosT) / (cosI + n * cosT);
		return (r_p * r_p + r_v * r_v) / 2.0;
		// refr = 1 - refl;
	}// else totally internal reflection
	return 1.0;
}

Color TestRenderer::directLight(const Ray& ray, Vec3f hitPoint, BSDF* bsdf){
	Color res;
	vector<Light* > lights = mScene->getLights();
	for (Light* light : lights){
		Vec3f lPos = light->samplePos();
		Vec3f L = hitPoint - lPos;
		real dist2 = L.L2();
		real dist = sqrt(dist2);
		L /= dist;
		real shade = mScene->visible(Ray(lPos, L), dist);
		if (shade > 0)
			res += light->getColor() / dist2 * bsdf->f(ray.d, L) * shade;
	}
	return res;
}

void TestRenderer::rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist){
	if (debug) ray.prt();
	Intersection isect;
	if (mScene->intersect(ray, isect) == MISS){
		res = BACKGROUND;
		return;
	}
	aDist = isect.getDist();
	Material* mat = isect.getPrim()->getMaterial();
	BSDF* bsdf = mat->buildBSDF(isect); //build BSDF
	res = directLight(ray, isect.getPos(), bsdf);
	return;
}

void TestRenderer::render(const Args& args){
	//assert(mCamera != NULL);
	vector<Ray> rays = mCamera->generateRays(args);
	int nRays = rays.size(), cnt = 0, tot = 10;
	int lastShow = -1;
	int w = mCamera->getFilm()->getW();
	int h = mCamera->getFilm()->getH();
	int* counter = new int[w * h];
	Color* resultColor = new Color[w * h];
	for (int i = 0; i < w * h; ++ i)
			resultColor[i] = BLACK, counter[i] = 0;
	cout << "number of pixels " << nRays << endl;

	#pragma omp parallel for
	//for (Ray ray : rays){
	for (int _ = 0; _ < rays.size(); ++ _){
		Ray ray = rays[_];
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
		resultColor[x * h + y] += res;
		counter[x * h + y] ++;
	}
	for (int i = 0; i < w; ++ i)
		for (int j = 0; j < h; ++ j){
			assert(counter[i * h + j] > 0);
			mCamera->getFilm()->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
		}
}


}; // namespace Raytracer
