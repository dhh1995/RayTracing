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
	Color getColor(UV uv);
	virtual Vec3f getNorm(Vec3f norm, UV uv){ return norm; }
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

// class BumpTexture : public Texture{
// 	Vec3f getNorm(Vec3f norm, UV uv){


// 	}
// };

}; // namespace Raytracer

#endif // CORE_TEXTURE_H
