// core/material.h
#ifndef CORE_MATERIAL_H
#define CORE_MATERIAL_H

#include "common.h"
#include "reflection.h"
#include "texture.h"
#include "sampler.h"

namespace Raytracer {

// Fresnel* noFresnel = new NoFresnel;

class Material{
public:
	Material(){
		mTexture = NULL;
		mFresnel = new NoFresnel;
	}
	Material(Color Ka, Color Kd, Color Ks, Fresnel* fresnel = NULL)
		: mAmb(Ka), mDiff(Kd), mSpec(Ks){
		if (fresnel == NULL)
			mFresnel = new NoFresnel;
		else
			mFresnel = fresnel;
		mTexture = NULL;
	}
	void	setAmbient(Color Ka){ mAmb = Ka; }
	Color 	getAmbient(){	return mAmb; }
	void	setDiffuse(Color Kd){ mDiff = Kd; }
	Color	getDiffuse(){	return mDiff; }
	void	setSpecular(Color Ks){ mSpec = Ks; }
	Color	getSpecular(){	return mSpec; }
	void	setFresnel(Fresnel* fresnel){ mFresnel = fresnel;}

	real getReflect(real cosI){
		return mFresnel->getReflection(cosI);
	}
	real getRefract(real cosI){
		return 1.0f - mFresnel->getReflection(cosI);
	}
	Color sample(const Vec3f& wi, Vec3f &wo, Vec3f norm, Vec3f pos, real& pdf, bool& isSpecular);

	void setTexture(Texture* aTexture){
		mTexture = aTexture;
	}
	Texture* getTexture(){
		return mTexture;
	}
	Color getColor(real u, real v){
		return mTexture->getColor(make_pair(u, v));
	}
	Vec3f getNorm(real u, real v){
		return ZERO;
	}
private:
	Color mAmb, mDiff, mSpec;
	Fresnel* mFresnel;
	Texture* mTexture;
};

}; // namespace Raytracer

#endif // CORE_MATERIAL_H
