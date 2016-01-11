// core/light.h
#ifndef CORE_LIGHT_H
#define CORE_LIGHT_H

#include "common.h"
#include "sampler.h"
#include "primitive.h"
#include "intersection.h"

namespace Raytracer {

class Light : public Primitive{
public:
	Light(Color aColor, Vec3f aPos, int aSamples = 1) : mColor(aColor), mPos(aPos), mSamples(aSamples){
	}
	virtual string getType(){
		return "PointLight";
	}
	bool intersect(const Ray& ray, Intersection& isect){
		return false;
	}
	Vec3f getPos(){
		return mPos;
	}
	Vec3f getColor(){
		return mColor;
	}
	Vec3f samplePos(){
		return mPos;
	}
	Vec3f sampleDir(){
		return Sampler::getRandomDir();
	}
protected:
	int mSamples;
	Vec3f mPos;
	Color mColor;
};

class AreaLight : public Light{
public:
	AreaLight(Vec3f aColor, Color aPos, Vec3f aNorm, Vec3f aU, int aSamples, real aUScale = 1.0, real aVScale = 1.0)
		: Light(aColor, aPos, aSamples), mNorm(aNorm), mU(aU), mUScale(aUScale), mVScale(aVScale){
			mNorm.Normalize();
			mU.Normalize();
			mV = cross(mNorm, mU).Normalize();
			mD = -dot(mPos, mNorm);
		}
	string getType(){
		return "AreaLight";
	}
	bool intersect(const Ray& ray, Intersection& isect);
	Vec3f samplePos(){
		real u, v;
		u = (Sampler::getRandReal() - 0.5) * mUScale;
		v = (Sampler::getRandReal() - 0.5) * mVScale;
		return mPos + mU * u + mV * v;
	}
private:
	Vec3f mNorm, mU, mV;
	real mD;
	real mUScale, mVScale; //how large
};

}; // namespace Raytracer

#endif // CORE_LIGHT_H
