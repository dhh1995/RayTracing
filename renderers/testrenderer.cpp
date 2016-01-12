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

Color TestRenderer::directLight(const Ray& ray, const Intersection& isect){
	Material* matter = isect.getPrim()->getMaterial();
	Vec3f hitPoint = isect.getPos();
	Vec3f N = isect.getNorm();
	Color color = isect.getColor();

	Color res; // = mAmbient * matter->getKa();
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
				Color diff = matter->getDiffuse();
				Color Ltemp;
				if (diff.L2() > 0){
					real dt = dot(L, N);
					if (dt > 0){
						Color diffuse = dt * diff;
						// add diffuse component to ray color
						Ltemp += diffuse * color * light->getColor();
					}
				}
				// determine specular component using Schlick's BRDF approximation
				Color spec = matter->getSpecular();
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

void TestRenderer::rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist){
	if (debug) ray.prt();
	Intersection isect;
	if (mScene->intersect(ray, isect) == MISS){
		res = BACKGROUND;
		return;
	}
	if (isect.isLight()){
		res = isect.getColor();
		return;
	}
	//real Li = mScene->getLi(isect);
	//printf("%lf\n",isect.getDist());
	//Vec3f pos = ray.o + ray.d * isect.getDist();
	//res = WHITE / (1 + (isect.getDist()));
	
	// res = color;
	// res = WHITE / ( 1 + aDist/ 300); //depth
	// return;

	aDist = isect.getDist();
	res = directLight(ray, isect);

	Material* matter = isect.getPrim()->getMaterial();
	Vec3f pos = isect.getPos();
	Vec3f norm = isect.getNorm();
	Color color = isect.getColor();


	// color.prt();
	// ray.prt();
	// printf("dist = %lf\n",aDist);
	// norm.prt();
	// pos.prt();

	bool totalReflection = false;
	// calculate refraction
	real refr = matter->getRefraction();
	if ((refr > 0.0f) && (depth < TRACEDEPTH))
	{
		real rindex = matter->getRefrIndex();
		real n = 1.0f / rindex;
		Vec3f N = norm;
		if (isect.isBack()){
			n = rindex;
			N = -N;
		}
		//printf("%lf\n",n);
		real cosI = -dot(N, ray.d);
		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
		if (cosT2 > 0.0f)
		{
			Vec3f T = (n * ray.d) + (n * cosI - sqrt( cosT2 )) * N;
			Color rcol( 0, 0, 0 );
			real dist;
			//printf("%d\n", isect.isBack());
			//puts("enter");
			//ray.prt();
			//Ray(pos + T * EPS, T).prt();
			rayTracing(Ray(pos + T * EPS, T), rcol, depth + 1, rindex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
			//puts("quit");
			mRaysCast++;
			// apply Beer's law
			Color absorbance = matter->getColor() * 0.1f * -dist;
			Color transparency = Color( exp( absorbance.r ), exp( absorbance.g ), exp( absorbance.b ) );
			Color add = refr * rcol;
			if (!isect.isBack())
				add = add * transparency;
			res += add; 
		}else
			totalReflection = true;
	}

	// calculate reflection
	real refl = matter->getReflection();
	if (totalReflection)
		refl = 1.0f;
	if ((refl > 0.0f) && (depth < TRACEDEPTH)){
		// calculate perfect reflection
		Vec3f N = isect.getNorm();
		Vec3f R = ray.d - 2.0f * dot(ray.d, N) * N;
		Color rcol(0, 0, 0);
		real dist;
		rayTracing( Ray( pos + R * EPS, R ), rcol, depth + 1, aRIndex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
		mRaysCast++;
		res += refl * rcol * color;
	}
	
	bool traceDiffuse = true;
	if (traceDiffuse){
		Color diff = matter->getDiffuse();
		if ((diff.L2() > 0) && (depth < TRACEDEPTH)){
			int nSample = 1;
			Color Ld;
			for (int i = 0; i < nSample; ++ i){
				Vec3f R = Sampler::getDiffuseDir(norm);
				Color rcol;
				real dist;
				rayTracing( Ray( pos + R * EPS, R ), rcol, depth + 1, aRIndex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
				mRaysCast++;
				Ld += rcol * color * diff * dot(R, norm);
			}
			res += Ld / nSample;
		}
	}
	//printf("res =");
	//res.prt();

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

	for (int iter = 0; iter < 100; ++ iter){
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
			//mCamera->getFilm()->setColor(x, y, res);
			resultColor[x * h + y] += res;
			counter[x * h + y] ++;
		}
		bool showImage = true;
		if (showImage){
			for (int i = 0; i < w; ++ i)
				for (int j = 0; j < h; ++ j){
					assert(counter[i * h + j] > 0);
					mCamera->getFilm()->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
				}
			show(false);
		}
	}
	for (int i = 0; i < w; ++ i)
		for (int j = 0; j < h; ++ j){
			assert(counter[i * h + j] > 0);
			mCamera->getFilm()->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
		}
}


}; // namespace Raytracer
