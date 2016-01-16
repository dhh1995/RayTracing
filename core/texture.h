// core/texture.h
#ifndef CORE_TEXTURE_H
#define CORE_TEXTURE_H

#include "common.h"
#include "film.h"

namespace Raytracer {

class Texture{
public:
	enum TYPE{
		IMAGE,
		BUMP,
	};
	Texture(Film* aImg, Vec3f aO, Vec3f aU, Vec3f aV, real aScaleU = 1.0, real aScaleV = 1.0)
		: mImg(aImg), mO(aO), mU(aU), mV(aV), mScaleU(aScaleU), mScaleV(aScaleV){
	}
	virtual Color getColor(Vec3f pos);
	virtual Vec3f getNorm(Vec3f pos, Vec3f norm){ return norm; }
protected:
	int mod(int x, int y){
		x %= y;
		if (x < 0)
			x += y;
		return x;
	}
	Film* mImg;
	int mType;
	Vec3f mO, mU, mV;
	real mScaleU, mScaleV;
};

class CheckerBoard : public Texture{
public:
	CheckerBoard(Color b, Color w, Vec3f aO, Vec3f aU, Vec3f aV, real aScaleU = 1.0, real aScaleV = 1.0)
		: Texture(NULL, aO, aU, aV, aScaleU, aScaleV), b(b), w(w){
	}
	Color getColor(Vec3f pos);
private:
	Color _getColor(int u, int v){
		return ((u + v) & 1) ? b : w;
	}
	Color b, w;
};

class BumpTexture : public Texture{
public:
	BumpTexture(Film* aImg,Vec3f aO, Vec3f aU, Vec3f aV, real aScaleU = 1.0, real aScaleV = 1.0, real aRate = 1.0):
		Texture(aImg, aO, aU, aV, aScaleU, aScaleV), mRate(aRate){
	}
	Vec3f getNorm(Vec3f pos, Vec3f norm);
private:
	real mRate;
};

}; // namespace Raytracer

#endif // CORE_TEXTURE_H
