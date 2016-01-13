#include "testrenderer.h"

bool debug = false;

namespace Raytracer {

Color TestRenderer::directLight(const Ray& ray, const Intersection& isect){
	Material* mtl = isect.getPrim()->getMaterial();
	Vec3f hitPoint = isect.getPos();
	Vec3f N = isect.getNorm();

	Color res = mScene->getAmbient() * mtl->getAmbient();
	vector<Light* > lights = mScene->getLights();
	for (Light* light : lights){
		if (!light->visible(hitPoint))
			continue;
		int nSamples = 1;//light->getNSamples();
		Color Ld;
		#pragma omp parallel for
		for (int i = 0; i < nSamples; ++ i){
			Vec3f lPos = light->samplePos();
			Vec3f L = hitPoint - lPos;
			real dist2 = L.L2();
			real dist = sqrt(dist2);
			L /= dist;
			real power = light->getPower(L);
			real shading = mScene->visible(Ray(lPos, L), dist);
			L = -L;
			if (shading > 0){
				Color diff = mtl->getDiffuse();
				Color Ltemp;
				if (diff.L2() > 0){
					real dt = dot(L, N);
					if (dt > 0){
						Color diffuse = dt * diff;
						// add diffuse component to ray color
						Ltemp += diffuse * light->getColor();
					}
				}
				// determine specular component using Schlick's BRDF approximation
				Color spec = mtl->getSpecular();
				if (spec.L2() > 0){
					// point light source: sample once for specular highlight
					Vec3f R = L - 2.0f * dot( L, N ) * N;
					real dt = dot(ray.d, R);	
					if (dt > 0){
						Color specular = dt * spec / (50 - 50 * dt + dt);
						// add specular component to ray color
						Ltemp += specular * light->getColor();
					}
				}
				Ld += Ltemp * (power * shading / dist2);
			}
		}
		res += Ld / nSamples;
	}
	return res;
}

void TestRenderer::rayTracing(Ray ray, Color& res, int depth){
	if (debug) ray.prt();
	Intersection isect;
	if (mScene->intersect(ray, isect) == MISS){
		res = BACKGROUND;
		return;
	}
	Primitive* prim = isect.getPrim();
	if (isect.isLight()){
		res = (static_cast<Light*>(prim))->getColor();
		return;
	}
	//real Li = mScene->getLi(isect);
	//printf("%lf\n",isect.getDist());
	//Vec3f pos = ray.o + ray.d * isect.getDist();
	//res = WHITE / (1 + (isect.getDist()));
	
	// aDist = isect.getDist();

	Material* mtl = prim->getMaterial();
	Vec3f pos = isect.getPos();
	Vec3f norm = isect.getNorm();

	res = directLight(ray, isect);

	// res = color;
	// res = WHITE / ( 1 + aDist/ 1.5); //depth
	// return;
	if (depth >= TRACEDEPTH)
		return;

	Color rcol;
	Vec3f R;
	real pdf;
	Color F = mtl->sample(ray.d, R, norm, pos, pdf);
	rayTracing(Ray(pos + R * EPS, R), rcol, depth + 1);
	res = rcol * F / pdf; 

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
		#pragma omp parallel for
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
