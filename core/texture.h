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
	Texture(Film* aImg, int aUScale = 1, int aVScale = 1){
		mImg = aImg;
		mUScale = aUScale;
		mVScale = aVScale;
	}
	Color getColor(real u, real v);
		//return mImg->getColor(mod(int(u * mUScale), mImg->w), mod(int(v * mVScale), mImg->h) );
private:
	int mod(int x, int y){
		x %= y;
		if (x < 0)
			x += y;
		return x;
	}
	Film* mImg;
	int mType;
	int mUScale, mVScale;
};

class ImgTexture : public Texture{


};

class BumpTexture : public ImgTexture{
	
	
};

}; // namespace Raytrace

#endif // CORE_TEXTURE_H
