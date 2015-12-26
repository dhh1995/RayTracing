// core/light.h
#ifndef CORE_LIGHT_H
#define CORE_LIGHT_H

#include "common.h"

namespace Raytracer {

class Light{
public:
	Light(Vec3f aPos, Color aColor) : mPos(aPos), mColor(aColor){
	}
	Vec3f getPos(){
		return mPos;
	}
	Vec3f getColor(){
		return mColor;
	}
protected:
	Vec3f mPos;
	Color mColor;
};

class AreaLight : public Light{
public:
	AreaLight(Vec3f aPos, Color aColor, Vec3f aNorm, Vec3f aI)
		: Light(aPos, aColor), mNorm(aNorm), mI(aI){
			mJ = cross(mNorm, mI);
		}
private:
	Vec3f mNorm, mI, mJ;
};

}; // namespace Raytracer

#endif // CORE_LIGHT_H
