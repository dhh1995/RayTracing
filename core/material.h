// core/material.h
#ifndef CORE_MATERIAL_H
#define CORE_MATERIAL_H

#include "common.h"
#include "reflection.h"
#include "texture.h"
#include "intersection.h"

namespace Raytracer {

class Material{
public:
	Material(){
		bsdf = new BSDF;
	}
	void setColor(Color &aColor){
		mColor = aColor;
	}
	bool haveTexture(){
		return mTexture != NULL;
	}
	Color getColor(){
		return mColor;
	}
	BSDF* getBSDF(){
		return bsdf;
	}
	BSDF* buildBSDF(const Vec3f& norm){
		BSDF* localBSDF = new BSDF(*bsdf);
		localBSDF->setNorm(norm);
		return localBSDF;
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
	Color mColor;
	Texture* mTexture;
	BSDF* bsdf;
};

}; // namespace Raytracer

#endif // CORE_MATERIAL_H
