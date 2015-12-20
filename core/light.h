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
private:
	Vec3f mPos;
	Color mColor;
};

}; // namespace Raytracer

#endif // CORE_LIGHT_H
