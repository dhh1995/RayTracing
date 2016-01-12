// renderers/photonrenderer.h
#ifndef CORE_PHOTONRENDER_H
#define CORE_PHOTONRENDER_H

#include "renderers/testrenderer.h"

namespace Raytracer {

const int PROGRESSIVE = 10;
const int PHOTONDEPTH = 10;
const int GLOBALWANT = 100000;
const int CAUSTICWANT = 0;
const real CAUSTIC_SEARCH_RADIUS = 1;
const real GLOBAL_SEARCH_RADIUS = 10;
const int SEARCH_PHOTONS = 200;

class PhotonRenderer : public TestRenderer {
public:
	PhotonRenderer(int globalWant = GLOBALWANT, int causticWant = CAUSTICWANT)
		: mPhotonEmits(0), mGlobalWant(globalWant), mCausticWant(causticWant){
	}
	void photonTracing(Photon* photon, int depth, bool meetSpecular);
	void genPhotonMap(const Args& args, string path);
	void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist);
	void render(const Args& args);
	//Options options;
private:
	void _addPhoton(Vec3f pos, Vec3f dir, Color power, bool meetSpecular);
	int mPhotonEmits, mGlobalWant, mCausticWant;
	int mGlobalFinish, mCausticFinish;
	PhotonMap mGlobal, mCaustic;
	int mProgress;
};

}; // namespace Raytracer

#endif // CORE_PHOTONRENDER_H
