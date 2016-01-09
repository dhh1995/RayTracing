// renderers/photonrenderer.h
#ifndef CORE_PHOTONRENDER_H
#define CORE_PHOTONRENDER_H

#include "renderers/testrenderer.h"

namespace Raytracer {

const int PHOTONDEPTH = 8;

class PhotonRenderer : public TestRenderer {
public:
	PhotonRenderer(int globalWant = 100000, int causticWant = 10000)
		: mPhotonEmits(0), mGlobalWant(globalWant), mCausticWant(causticWant){
	}
	void photonTracing(Photon* photon, int depth, bool meetSpecular);
	void genPhotonMap(string path);
	void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist);
	void render(const Args& args);
	//Options options;
private:
	void _addPhoton(Vec3f pos, Vec3f dir, Color power, bool meetSpecular);
	int mPhotonEmits, mGlobalWant, mCausticWant;
	int mGlobalFinish, mCausticFinish;
	PhotonMap mGlobal, mCaustic;
};

}; // namespace Raytracer

#endif // CORE_PHOTONRENDER_H
