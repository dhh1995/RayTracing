// core/material.h
#ifndef CORE_MATERIAL_H
#define CORE_MATERIAL_H

#include "common.h"
#include "reflection.h"
#include "texture.h"

namespace Raytracer {

// Fresnel* noFresnel = new NoFresnel;

class Material{
public:
	Material(){
		mTexture = NULL;
	}
	Material(Color aColor, real aRefl, real aRefr, Color aDiff, Color aSpec, real aRIndex = 1, Color Ka = BLACK)
		: mColor(aColor), mRefl(aRefl), mRefr(aRefr), mDiff(aDiff), mSpec(aSpec), mRIndex(aRIndex), mKa(Ka){
		mTexture = NULL;
	}
	void setColor(const Color &aColor){
		mColor = aColor;
	}
	bool haveTexture(){
		return mTexture != NULL;
	}
	Color getColor(){
		return mColor;
	}
	void setDiffuse(Color aDiff){
		mDiff = aDiff;
	}
	Color getDiffuse(){
		return mDiff;
	}
	void setSpecular(Color aSpec){
		mSpec = aSpec;
	}
	Color getSpecular(){
		return mSpec;
	}
	void setReflection(real aRefl){
		mRefl = aRefl;
	}
	real getReflection(){
		return mRefl;
	}
	void setRefraction(real aRefr){
		mRefl = aRefr;
	}
	real getRefraction(){
		return mRefr;
	}
	void setRefrIndex(real aRIndex){
		mRIndex = aRIndex;
	}
	real getRefrIndex(){
		return mRIndex;
	}
	void setKa(Color Ka){
		mKa = Ka;
	}
	Color getKa(){
		return mKa;
	}
	void setTexture(Texture* aTexture){
		mTexture = aTexture;
	}
	Texture* getTexture(){
		return mTexture;
	}
	Color getColor(real u, real v){
		return mTexture->getColor(u, v);
	}
	Vec3f getNorm(real u, real v){
		return ZERO;
	}
private:
	Color mKa;
	Color mColor;
	Color mDiff, mSpec;
	real mRefl, mRefr;
	real mRIndex;
	Texture* mTexture;
	// Fresnel* fresnel;
};

}; // namespace Raytracer

#endif // CORE_MATERIAL_H
