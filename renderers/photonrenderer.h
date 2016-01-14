// renderers/photonrenderer.h
#ifndef CORE_PHOTONRENDER_H
#define CORE_PHOTONRENDER_H

#include "renderers/testrenderer.h"

namespace Raytracer {

const int PHOTONDEPTH = 10;
const int GLOBALWANT = 1000;
const int CAUSTICWANT = 100;
const real CAUSTIC_SEARCH_RADIUS = 1;
const real GLOBAL_SEARCH_RADIUS = 10;
const int SEARCH_PHOTONS = 200;

enum class PhotonState : int {
	Null	  = 0x0001,
    Direct    = 0x0100,
    Indirect  = 0x0101,
    Caustic   = 0x0200,
};

class PhotonRenderer : public TestRenderer {
public:
	PhotonRenderer(int globalWant = GLOBALWANT, int causticWant = CAUSTICWANT)
		: mPhotonEmits(0), mGlobalWant(globalWant), mCausticWant(causticWant){
	}
	void photonTracing(Photon* photon, int depth, PhotonState state);
	void genPhotonMap(const Args& args, string path);
	void rayTracing(Ray ray, Color& res, int depth);
	void render(const Args& args);
	//Options options;
private:
	void _addPhoton(Vec3f pos, Vec3f dir, Color power, PhotonState state);
	int mPhotonEmits, mGlobalWant, mCausticWant;
	int mGlobalFinish, mCausticFinish;
	PhotonMap mGlobal, mCaustic;
	int mProgress;
};

}; // namespace Raytracer

#endif // CORE_PHOTONRENDER_H
