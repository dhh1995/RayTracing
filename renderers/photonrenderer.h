// renderers/photonrenderer.h
#ifndef CORE_PHOTONRENDER_H
#define CORE_PHOTONRENDER_H

#include "renderers/testrenderer.h"

namespace Raytracer {

class PhotonRenderer : public TestRenderer {
public:
	PhotonRenderer() : mPhotonEmits(0){
	}
	void photonTracing(PhotonMap &pm, Photon* photon);
	void genGlobalPhotonMap(string path, int require);
	void genCausticPhotonMap(string path, int require);
	//void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist);
	void render(const Args& args);
	//Options options;
private:
	int mPhotonEmits;
	PhotonMap mGlobal, mCaustic;
};

}; // namespace Raytracer

#endif // CORE_PHOTONRENDER_H
