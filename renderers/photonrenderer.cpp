#include "photonrenderer.h"

namespace Raytracer {

void PhotonRenderer::_addPhoton(Vec3f pos, Vec3f dir, Color power, PhotonState state){
	if (state == PhotonState::Caustic){
		if (mCaustic.getN() < mCausticWant){
			// puts("photon");
			// pos.prt();
			// dir.prt();
			mCaustic.add(new Photon(Ray(pos, dir), power));
			if (mCaustic.getN() == mCausticWant)
				mCausticFinish = mPhotonEmits;
		}
	}else{
		if (mGlobal.getN() < mGlobalWant){
			//photon->prt();
			mGlobal.add(new Photon(Ray(pos, dir), power));
			if (mGlobal.getN() == mGlobalWant)
				mGlobalFinish = mPhotonEmits;
		}
	}
}

void PhotonRenderer::photonTracing(Photon* photon, int depth, PhotonState state){
	if (depth >= PHOTONDEPTH)
		return;
	Intersection isect;
	Ray ray = photon->getRay();
	if (mScene->intersect(ray, isect) == MISS)
		return;
	Material* mtl = isect.getPrim()->getMaterial();
	Vec3f pos = isect.getPos();
	Vec3f norm = isect.getNorm();
	Color P = photon->getPower();

	bool isSpecular;
	Vec3f R;
	real pdf;
	Color F = mtl->sample(ray.d, R, norm, pos, pdf, isSpecular);
	PhotonState newState = state;
    if (!isSpecular){
    	if (state == PhotonState::Null)
    		newState = PhotonState::Direct;
        newState = PhotonState::Indirect;
    } else
        newState = PhotonState::Caustic;

	if (mtl->getDiffuse().getMax() > EPS)
		_addPhoton(pos, ray.d, P, newState);

	photon->setRay(Ray(pos + EPS * R, R));
	photon->updatePower(F / pdf);
	photonTracing(photon, depth + 1, newState);
}

void PhotonRenderer::genPhotonMap(const Args& args, string path){
	string causticPmFileName = path + "caustic.pm";
	string globalPmFileName = path + "global.pm";
	FILE* causticPmFile = fopen(causticPmFileName.c_str(), "r");
	FILE* globalPmFile  = fopen( globalPmFileName.c_str(), "r");
	if (args.loadPM){
		if (causticPmFile != NULL){
			fscanf(causticPmFile, "%d", &mCausticFinish);
			mCaustic.load(causticPmFile);
		}
		if (globalPmFile  != NULL){
			fscanf(globalPmFile, "%d", &mGlobalFinish);
			mGlobal.load(globalPmFile);
		}
	}else{
		mGlobal.clear();
		mCaustic.clear();
	}

	mPhotonEmits = 0;
	int m = mScene->getLights().size();
	int total = mGlobalWant + mCausticWant, cnt = 0, last = 1, showStages = 10;
	vector<Light* > lights = mScene->getLights();
	while (mGlobal.getN() < mGlobalWant || mCaustic.getN() < mCausticWant){
		int ind = Sampler::getRandInt(lights.size());
		Light* light = lights[ind];
		Vec3f centre = light->samplePos();
		Vec3f dir = Sampler::getRandomDir();
		Photon* photon = new Photon(Ray(centre, dir), light->getPower());
		//puts("new Photon emits");
		mPhotonEmits++;
		photonTracing(photon, 0, PhotonState::Null);
		delete photon;
		if (mGlobal.getN() + mCaustic.getN() >= last * total / showStages)
			printf("photon emits %d/%d global : %d caustic : %d\n", last++, showStages, mGlobal.getN(), mCaustic.getN());
	}
	if (!args.loadPM){
		causticPmFile = fopen(causticPmFileName.c_str(), "w");
		fprintf(causticPmFile, "%d\n", mCausticFinish);
		mCaustic.dump(causticPmFile);
	}
	if (!args.loadPM){
		globalPmFile = fopen(globalPmFileName.c_str(), "w");
		fprintf(globalPmFile, "%d\n", mGlobalFinish);
		mGlobal.dump(globalPmFile);
	}
	mGlobal.construct();
	mCaustic.construct();
}

Color PhotonRenderer::_getFlux(const PhotonMap& pm, Vec3f pos, Vec3f norm, real radius){
	int nPhotons = SEARCH_PHOTONS;
	pair<real, Photon* > * photons = new pair<real, Photon* >[nPhotons + 5];
	int m = pm.getKNearest(pos, nPhotons, photons, radius);

	Color flux;
	real maxDist2 = -1;
	vector<Photon* > valid;
	for (int i = 0; i < m; ++ i){
		Photon* photon = photons[i].second;
		Vec3f diff = (pos - photon->getPos());
		real dist = diff.length();
		real  dt = dot(norm, diff) / dist;
		if (dot(norm, photon->getRay().d) < 0 && dt < radius * radius * 0.1){
			valid.push_back(photon);
			maxDist2 = max(maxDist2, photons[i].first);
		}
	}
	real k = 1.1;
	if (valid.size() >= 5){
		real maxDist = sqrt(maxDist2);
		for (Photon* photon : valid){
			real dist = (pos - photon->getPos()).length();
			real weight = 1.0f - dist / (k * maxDist);
			flux += photon->getPower() * (weight / PI);
		}
	}
	delete[] photons;
	flux /= (1.0 - 2.0 / (3.0 * k));
	return flux / (PI * maxDist2);
}

void PhotonRenderer::rayTracing(Ray ray, Color& res, int depth){
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
	Material* mtl = prim->getMaterial();
	Vec3f pos = isect.getPos();
	Vec3f norm = isect.getNorm();

	if (depth >= TRACEDEPTH)
		return;

	bool isSpecular;
	Vec3f R;
	real pdf;
	Color F = mtl->sample(ray.d, R, norm, pos, pdf, isSpecular);
	if (isSpecular){
		Color rcol;
		rayTracing(Ray(pos + R * EPS, R), rcol, depth + 1);
		res += F * rcol / pdf;
	}else{
		Color caustic = _getFlux(mCaustic, pos, norm, mCausticR);
		Color radiance = _getFlux(mGlobal, pos, norm, mGlobalR);
		res += F * (caustic + radiance) / pdf;
	}
}

void PhotonRenderer::render(const Args& args){
	// char names[105];
	// sprintf(names, "PhotonMaps/scene%d", args.useScene);
	// genPhotonMap(args, names);
	// progressMessage("Generate photon maps done.");

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
	for (int iter = 0; iter < args.photonIter; ++ iter){
		mCausticR = CAUSTIC_SEARCH_RADIUS;
		mGlobalR = GLOBAL_SEARCH_RADIUS;
		mCausticR *= (iter + DECAY) / (iter + 1);
		mGlobalR *= (iter + DECAY) / (iter + 1);
		genPhotonMap(args, "");
		#pragma omp parallel for
		for (int pixel = 0; pixel < nPixels; ++ pixel){
			int x = pixel / h, y = pixel - x * h;
			Ray ray = mCamera->sampleRay(x, y);
			int percent = (++cnt) * tot / nPixels;
			if (percent != lastShow){
				lastShow = percent;
				printf("%d/%d\n", percent,tot);
			}
			Color res;
			rayTracing(ray, res, 0);
			resultColor[pixel] += res;
			counter[pixel] ++;
		}
		bool snapshot = true;
		if (snapshot)
			_showImage(film, w, h, resultColor, counter);
	}
	_showImage(film, w, h, resultColor, counter, false);
}

}; // namespace Raytracer
