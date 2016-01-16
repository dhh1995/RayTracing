#include "testrenderer.h"

bool debug = false;

namespace Raytracer {

void TestRenderer::rayTracing(Ray ray, Color& res, int depth){
	if (debug) ray.prt();
	Intersection isect;
	if (mScene->intersect(ray, isect, true) == MISS){
		res = BACKGROUND;
		return;
	}
	Primitive* prim = isect.getPrim();
	if (isect.isLight()){
		res = (static_cast<Light*>(prim))->getPower();
		return;
	}

	//real Li = mScene->getLi(isect);
	//printf("%lf\n",isect.getDist());
	//Vec3f pos = ray.o + ray.d * isect.getDist();
	// res = WHITE / (1 + (isect.getDist()));
	// res = isect.getPrim()->getMaterial()->getDiffuse();
	// return;
	
	// aDist = isect.getDist();
	// ray.prt();
	// printf("%d\n",prim);
	Material* mtl = prim->getMaterial();
	Vec3f pos = isect.getPos();
	Vec3f norm = isect.getNorm();
	res = mScene->getAmbient() * mtl->getAmbient();
	// res = WHITE / ( 1 + isect.getDist()/ 1.5); //depth
	// return;
	if (depth >= TRACEDEPTH)
		return;

	Color rcol;
	Vec3f R;
	real pdf;
	bool isSpecular;
	Color F = mtl->sample(ray.d, R, norm, pos, pdf, isSpecular);
	rayTracing(Ray(pos + R * EPS, R), rcol, depth + 1);
	res += rcol * F / pdf; 

	return;
}

void TestRenderer::render(const Args& args){
	//assert(mCamera != NULL);
	Film* film = mCamera->getFilm();
	int lastShow = -1;
	int w = film->getW();
	int h = film->getH();
	int nPixels = w * h, cnt = 0, tot = 10;
	int* counter = new int[nPixels];
	Color* resultColor = new Color[nPixels];
	for (int i = 0; i < nPixels; ++ i)
			resultColor[i] = BLACK, counter[i] = 0;
	cout << "number of pixels " << nPixels << endl;

	for (int iter = 0; iter < args.pathIter; ++ iter){
		// #pragma omp parallel for
		for (int pixel = 0; pixel < nPixels; ++ pixel){
			int x = pixel / h, y = pixel - x * h;
			Ray ray = mCamera->sampleRay(x, y);
			// ray.prt();
			int percent = (++cnt) * tot / nPixels;
			if (percent != lastShow){
				lastShow = percent;
				printf("%d/%d\n", percent,tot);
			}
			Color res;
			real dist;
			rayTracing(ray, res, 0);
			// res.prt();
			//dist.prt();
			resultColor[pixel] += res;
			counter[pixel] ++;
		}
		bool snapshot = true;
		if (snapshot)
			_showImage(film, w, h, resultColor, counter);
	}
	_showImage(film, w, h, resultColor, counter, false);
}

void TestRenderer::_showImage(Film* film, int w, int h, Color* resultColor, int* counter, bool temp){
	for (int i = 0; i < w; ++ i)
		for (int j = 0; j < h; ++ j){
			assert(counter[i * h + j] > 0);
			film->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
		}
	show(false);
}

}; // namespace Raytracer
