// core/light.h
#ifndef CORE_LIGHT_H
#define CORE_LIGHT_H

#include "common.h"
#include "primitive.h"
#include "intersection.h"
#include "sampler.h"

namespace Raytracer {

class Light : public Primitive{
public:
	Light(Color aColor, Vec3f aPos, real aPower = 1.0, int aSamples = 1)
		: mColor(aColor), mPower(aPower), mPos(aPos), mSamples(aSamples){
	}
	virtual string getType(){
		return "PointLight";
	}
	virtual bool visible(Vec3f pos){
		return true;
	}
	bool intersect(const Ray& ray, Intersection& isect){
		return false;
	}
	Vec3f getPos(){
		return mPos;
	}
	virtual Color getPower(){
		return mColor * mPower;
	}
	virtual real getPower(Vec3f dir){
		return mPower;
	}
	Color getColor(){
		return mColor;
	}
	int getNSamples(){
		return mSamples;
	}
	virtual Vec3f samplePos(){
		return mPos;
	}
	virtual Vec3f sampleDir(){
		return Sampler::getRandomDir();
	}
protected:
	int mSamples;
	Vec3f mPos;
	Color mColor;
	real mPower;
};

class AreaLight : public Light{
public:
	AreaLight(Color aColor, Color aPos, real aPower, Vec3f aNorm, Vec3f aU,
				int aSamples, real aUScale = 1.0, real aVScale = 1.0)
			: Light(aColor, aPos, aPower, aSamples), mNorm(aNorm), mU(aU), mUScale(aUScale), mVScale(aVScale){
		aColor.prt();
		mNorm.Normalize();
		mU.Normalize();
		mV = cross(mNorm, mU).Normalize();
		mD = -dot(mPos, mNorm);
	}
	string getType(){
		return "AreaLight";
	}
	bool visible(Vec3f pos){
		return dot(mNorm, pos - mPos) > 0;
	}
	virtual Color getPower(){
		return mColor * mPower;
	}
	// virtual real getPower(Vec3f dir){
	// 	return mPower * dot(mNorm, dir) / PI;
	// }
	bool intersect(const Ray& ray, Intersection& isect);
	Vec3f samplePos(){
		real u, v;
		u = (Sampler::getRandReal() - 0.5) * mUScale;
		v = (Sampler::getRandReal() - 0.5) * mVScale;
		return mPos + mU * u + mV * v;
	}
	Vec3f sampleDir(){
		return Sampler::getDiffuseDir(mNorm);
	}
private:
	Vec3f mNorm, mU, mV;
	real mD;
	real mUScale, mVScale; //how large
};

class ProjectLight : public Light{
public:
	ProjectLight(Color aColor, Color aPos, real aPower, Vec3f aNorm, real aR)
		: Light(aColor, aPos, aPower), mNorm(aNorm), mR(aR){
		while (mU.L2() < EPS)
			mU = cross(mNorm, Sampler::randVector());
		mU.Normalize();
		mV = cross(mU, mNorm).Normalize();
	}
	string getType(){
		return "ProjectLight";
	}
	Vec3f samplePos(){
		real u, v;
		Sampler::concentricSampleDisk(u, v);
		return mPos + (mU * u + mV * v) * mR;
	}
	Vec3f sampleDir(){
		return mNorm;
	}
private:
	Vec3f mNorm, mU, mV;
	real mR;
};


}; // namespace Raytracer

#endif // CORE_LIGHT_H
