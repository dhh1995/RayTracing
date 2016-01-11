// core/light.h
#ifndef CORE_LIGHT_H
#define CORE_LIGHT_H

#include "common.h"

namespace Raytracer {

class Light{
public:
	Light(Color aColor, Vec3f aPos) : mColor(aColor), mPos(aPos){
	}
	virtual string getType(){
		return "Point";
	}
	Vec3f getPos(){
		return mPos;
	}
	virtual Vec3f getPos(real u, real v){
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
	AreaLight(Vec3f aColor, Color aPos, Vec3f aNorm, Vec3f aU)
		: Light(aColor, aPos), mNorm(aNorm), mU(aU){
			mNorm.Normalize();
			mU.Normalize();
			mV = cross(mNorm, mU).Normalize();
		}
	string getType(){
		return "Area";
	}
	Vec3f getPos(real u, real v){
		return mPos + mU * u + mV * v;
	}
private:
	Vec3f mNorm, mU, mV;
};

}; // namespace Raytracer

#endif // CORE_LIGHT_H
