// core/photon.h
#ifndef CORE_PHOTON_H
#define CORE_PHOTON_H

#include "common.h"

namespace Raytracer {

class Photon{
public:
	Photon();
	Vec3f getPos() const{
		return mPos;
	}
	real getAxis(int dim) const{
		return mPos[dim];
	}
private:
	Vec3f mPos;
};



}; // namespace Raytrace

#endif // CORE_PHOTON_H
