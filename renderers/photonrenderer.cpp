// #include "photonrenderer.h"

// namespace Raytracer {

// void PhotonRenderer::_addPhoton(Vec3f pos, Vec3f dir, Color power, bool meetSpecular){
// 	if (meetSpecular){
// 		if (mCaustic.getN() < mCausticWant){
// 			// puts("photon");
// 			// pos.prt();
// 			// dir.prt();
// 			mCaustic.add(new Photon(Ray(pos, dir), power));
// 			if (mCaustic.getN() == mCausticWant)
// 				mCausticFinish = mPhotonEmits;
// 		}
// 	}else{
// 		if (mGlobal.getN() < mGlobalWant){
// 			//photon->prt();
// 			mGlobal.add(new Photon(Ray(pos, dir), power));
// 			if (mGlobal.getN() == mGlobalWant)
// 				mGlobalFinish = mPhotonEmits;
// 		}
// 	}
// }

// void PhotonRenderer::photonTracing(Photon* photon, int depth, bool meetSpecular){
// 	if (depth >= PHOTONDEPTH)
// 		return;
// 	Intersection isect;
// 	Ray ray = photon->getRay();
// 	if (mScene->intersect(ray, isect) == MISS)
// 		return;
// 	Material* matter = isect.getPrim()->getMaterial();
// 	Vec3f pi = isect.getPos();
// 	Vec3f norm = isect.getNorm();
// 	Color color = isect.getColor();

// 	real diff = matter->getDiffuse();
// 	real spec = matter->getSpecular();
// 	// real refr = matter->getRefraction();
// 	// real refl = matter->getReflection();
// 	real ran = Sampler::getRandReal();
// 	// cout << isect.getPrim()->getType() << endl;
// 	// ray.prt();
// 	// printf("%lf %lf\n", ran , spec);

// 	//printf("%lf %lf\n",ran, spec);
// 	if (ran < spec){
// 		//spec, reflection or refraction
// 		real rindex = matter->getRefrIndex();
// 		real n = 1.0f / rindex;
// 		Vec3f N = norm;
// 		if (isect.isBack()){
// 			n = rindex;
// 			N = -N;
// 		}
// 		real cosI = -dot(N, ray.d);
// 		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
// 		//printf("%lf\n",cosT2);
// 		//real refl = _FresnelReflection(n, cosI, cosT2);
// 		real refl = matter->getReflection();
// 		if (cosT2 < 0)
// 			refl = 1.;
// 		Vec3f R;
// 		if (ran < refl * spec)
// 			R = ray.d - 2.0f * dot(ray.d, N) * N;
// 		else{
// 			R = (n * ray.d) + (n * cosI - sqrt( cosT2 )) * N;
// 		}
// 		meetSpecular = true;
// 		photon->setRay(Ray(pi + EPS * R, R));
// 		photon->updatePower(color);
// 		photonTracing(photon, depth + 1, meetSpecular);
// 	}else if (ran < diff + spec){
// 		// diffuse, get new direction and save it in photon map
// 		_addPhoton(pi, ray.d, photon->getPower(), meetSpecular);
// 		Vec3f dir = _getDiffuseDir(norm);
// 		photon->setRay(Ray(pi + EPS * dir, dir));
// 		photon->updatePower(color);
// 		photonTracing(photon, depth + 1, false);
// 	}else{
// 		_addPhoton(pi, ray.d, photon->getPower(), meetSpecular);
// 	}
// }

// void PhotonRenderer::genPhotonMap(const Args& args, string path){
// 	string causticPmFileName = path + "caustic.pm";
// 	string globalPmFileName = path + "global.pm";
// 	FILE* causticPmFile = fopen(causticPmFileName.c_str(), "r");
// 	FILE* globalPmFile  = fopen( globalPmFileName.c_str(), "r");
// 	if (args.loadPM){
// 		if (causticPmFile != NULL){
// 			fscanf(causticPmFile, "%d", &mCausticFinish);
// 			mCaustic.load(causticPmFile);
// 		}
// 		if (globalPmFile  != NULL){
// 			fscanf(globalPmFile, "%d", &mGlobalFinish);
// 			mGlobal.load(globalPmFile);
// 		}
// 	}else{
// 		mGlobal.clear();
// 		mCaustic.clear();
// 	}

// 	mPhotonEmits = 0;
// 	int m = mScene->getLights().size();
// 	int total = mGlobalWant + mCausticWant, cnt = 0, last = 1, showStages = 10;
// 	vector<Light* > lights = mScene->getLights();
// 	while (mGlobal.getN() < mGlobalWant || mCaustic.getN() < mCausticWant){
// 		int ind = Sampler::getRandInt(lights.size());
// 		Light* light = lights[ind];
// 		if (light->getType() == "Point"){
// 			Vec3f centre = light->getPos();
// 			Vec3f dir = _getRandomDir();
// 			Photon* photon = new Photon(Ray(centre, dir), light->getColor());
// 			//puts("new Photon emits");
// 			mPhotonEmits++;
// 			photonTracing(photon, 0, false);
// 			delete photon;
// 		}
// 		if (mGlobal.getN() + mCaustic.getN() >= last * total / showStages)
// 			printf("photon emits %d/%d global : %d caustic : %d\n", last++, showStages, mGlobal.getN(), mCaustic.getN());
// 	}
// 	if (!args.loadPM){
// 		causticPmFile = fopen(causticPmFileName.c_str(), "w");
// 		fprintf(causticPmFile, "%d\n", mCausticFinish);
// 		mCaustic.dump(causticPmFile);
// 	}
// 	if (!args.loadPM){
// 		globalPmFile = fopen(globalPmFileName.c_str(), "w");
// 		fprintf(globalPmFile, "%d\n", mGlobalFinish);
// 		mGlobal.dump(globalPmFile);
// 	}
// 	mGlobal.construct();
// 	mCaustic.construct();
// }

// void PhotonRenderer::rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist){
// 	Intersection isect;
// 	if (mScene->intersect(ray, isect) == MISS){
// 		res = BACKGROUND;
// 		return;
// 	}
// 	aDist = isect.getDist();
// 	Material* matter = isect.getPrim()->getMaterial();
// 	Vec3f pi = isect.getPos();
// 	Vec3f norm = isect.getNorm();
// 	Color color = isect.getColor();
// 	//res = mScene->getLi(ray, isect);
// 	res = BLACK;
// 	// need to gather photon.
// 	real diff = matter->getDiffuse();
// 	// puts("------------------");
// 	// ray.prt();
// 	// pi.prt();
// 	// printf("%lf\n",diff);
// 	if (diff > 0){
// 		Color radiance, caustic;
// 		int nPhotons = SEARCH_PHOTONS;
// 		real alpha = CAUSTIC_SEARCH_RADIUS, beta = GLOBAL_SEARCH_RADIUS;
// 		pair<real, Photon* > * photons = new pair<real, Photon* >[nPhotons + 5];
// 		{
// 			int m = mCaustic.getKNearest(pi, nPhotons, photons, (alpha + mProgress) / (1. + mProgress));
// 			if (m >= 8){
// 				real radius2 = photons[0].first;
// 				for (int i = 0; i < m; ++ i){
// 					Photon* photon = photons[i].second;
// 					if (dot(norm, photon->getRay().d) < 0)
// 						caustic += photon->getPower();
// 				}
// 				caustic /= 0 * mCaustic.getN() + 1 * mCausticFinish;
// 				caustic /= PI * radius2;
// 			}
// 		}
// 		{
// 			int m = mGlobal.getKNearest(pi, nPhotons, photons, (beta + mProgress) / (1. + mProgress));
// 			if (m >= 1){
// 				real radius2 = photons[0].first;
// 				for (int i = 0; i < m; ++ i){
// 					Photon* photon = photons[i].second;
// 					if (dot(norm, photon->getRay().d) < 0)
// 						radiance += photon->getPower();
// 				}
// 				radiance /= 0 * mGlobal.getN() + 1 * mGlobalFinish;
// 				radiance /= PI * radius2;
// 			}
// 		}
// 		delete[] photons;
// 		// printf("radiance ");
// 		// radiance.prt();
// 		res += caustic * diff;
// 		res += radiance * diff;
// 		res *= color;
// 	}
// 	// if (showPhoton){
// 	// 	real dist;
// 	// 	Color rcol;
// 	// 	rayTracing(Ray(pi + ray.d * EPS, ray.d), rcol, depth + 1, 1, dist);
// 	// 	res += rcol;
// 	// 	return;
// 	// }

// 	// calculate specular
// 	//real refr = matter->getRefraction();
// 	real spec = matter->getSpecular();
// 	if ((spec > 0.0f) && (depth < TRACEDEPTH))
// 	{
// 		real rindex = matter->getRefrIndex();
// 		real n = 1.0f / rindex;
// 		Vec3f N = norm;
// 		if (isect.isBack()){
// 			n = rindex;
// 			N = -N;
// 		}
// 		//printf("%lf\n",n);
// 		real cosI = -dot(N, ray.d);
// 		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
// 		// real refl = _FresnelReflection(n, cosI, cosT2) * spec;
// 		// real refr = spec - refl;
// 		real refl = matter->getReflection();
// 		real refr = matter->getRefraction();
// 		if (cosT2 < 0.0f)
// 			refl = 1, refr = 0;

// 		if ((refr > 0.0f) && (depth < TRACEDEPTH)){
// 			Vec3f T = (n * ray.d) + (n * cosI - sqrt( cosT2 )) * N;
// 			Color rcol( 0, 0, 0 );
// 			real dist;
// 			rayTracing(Ray(pi + T * EPS, T), rcol, depth + 1, rindex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
// 			mRaysCast++;
// 			// apply Beer's law
// 			Color absorbance = matter->getColor() * 0.1f * -dist;
// 			Color transparency = Color( exp( absorbance.r ), exp( absorbance.g ), exp( absorbance.b ) );
// 			Color add = refr * rcol;
// 			// if (!isect.isBack())
// 			// 	add = add * transparency;
// 			res += add;
// 		}

// 		if ((refl > 0.0f) && (depth < TRACEDEPTH)){
// 			// calculate perfect reflection
// 			Vec3f N = isect.getNorm();
// 			Vec3f R = ray.d - 2.0f * dot(ray.d, N) * N;
// 			Color rcol(0, 0, 0);
// 			real dist;
// 			rayTracing( Ray( pi + R * EPS, R ), rcol, depth + 1, aRIndex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
// 			mRaysCast++;
// 			res += refl * rcol * color;
// 		}
// 	}

// 	return;
// }

// void PhotonRenderer::render(const Args& args){
// 	char names[105];
// 	sprintf(names, "PhotonMaps/scene%d", args.useScene);
// 	//genPhotonMap(args, names);
// 	//genCausticPhotonMap("caustic", 100);
// 	progressMessage("Generate photon maps done.");

// 	//assert(mCamera != NULL);
// 	vector<Ray> rays = mCamera->generateRays(args);
// 	int nRays = rays.size(), cnt = 0, tot = 10;
// 	int lastShow = -1;
// 	int w = mCamera->getFilm()->getW();
// 	int h = mCamera->getFilm()->getH();
// 	int* counter = new int[w * h];
// 	Color* resultColor = new Color[w * h];
// 	for (int i = 0; i < w * h; ++ i)
// 			resultColor[i] = BLACK, counter[i] = 0;
// 	cout << "number of pixels " << nRays << endl;

// 	int progressive = PROGRESSIVE;
// 	for (mProgress = 0; mProgress < progressive; ++ mProgress){
// 		genPhotonMap(args, "");
// 		#pragma omp parallel for
// 		//for (Ray ray : rays){
// 		for (int _ = 0; _ < rays.size(); ++ _){
// 			Ray ray = rays[_];
// 			++cnt;
// 			int percent = cnt * tot / nRays;
// 			if (percent != lastShow){
// 				lastShow = percent;
// 				printf("%d/%d\n", percent,tot);
// 			}

// 			int x = ray.mFilmX, y = ray.mFilmY;
// 			Color res;
// 			real dist;
// 			rayTracing(ray, res, 0, 1, dist);
// 			//mCamera->getFilm()->setColor(x, y, res);
// 			resultColor[x * h + y] += res / progressive;
// 			counter[x * h + y] ++;
// 		}
// 		bool showImg = true;
// 		if (showImg){
// 			for (int i = 0; i < w; ++ i)
// 				for (int j = 0; j < h; ++ j){
// 					assert(counter[i * h + j] > 0);
// 					mCamera->getFilm()->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
// 				}
// 			show();
// 		}
// 	}
// 	for (int i = 0; i < w; ++ i)
// 		for (int j = 0; j < h; ++ j){
// 			assert(counter[i * h + j] > 0);
// 			mCamera->getFilm()->setColor(i, j, resultColor[i * h + j] / counter[i * h + j]);
// 		}
// }

// }; // namespace Raytracer
