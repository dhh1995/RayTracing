// renderers/photonrenderer.h
#ifndef CORE_PHOTONRENDER_H
#define CORE_PHOTONRENDER_H

#include "renderers/testrenderer.h"

namespace Raytracer {

const int PHOTONDEPTH = 10;

enum class PhotonState : int {
	// Null	  = 0x0001,
    Direct    = 0x0100,
    Indirect  = 0x0101,
    Caustic   = 0x0200,
};

class PhotonRenderer : public TestRenderer {
public:
	PhotonRenderer(int globalWant, int causticWant)
		: mPhotonEmits(0), mGlobalWant(globalWant), mCausticWant(causticWant){
	}
	void photonTracing(Photon* photon, int depth, PhotonState state);
	void genPhotonMap(const Args& args, string path);
	void rayTracing(Ray ray, Color& res, int depth);
	void render(const Args& args);
	//Options options;
private:
	Color _kernel(Color power, real dist, real maxDist);
	Color _getFlux(const PhotonMap& pm, Vec3f pos, Vec3f norm, real radius);
	void _addPhoton(Vec3f pos, Vec3f dir, Color power, PhotonState state);
	int mPhotonEmits, mGlobalWant, mCausticWant;
	int mGlobalFinish, mCausticFinish;
	PhotonMap mGlobal, mCaustic;
	real mGlobalR, mCausticR;
	int mPhotons;
	real mBias;
};

}; // namespace Raytracer

#endif // CORE_PHOTONRENDER_H
