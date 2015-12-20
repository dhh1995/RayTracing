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
	Color getColor(real u, real v){
		return mImg->getColor(mod(int(u * mImg->w), mImg->w), mod(int(v * mImg->h), mImg->h) );
	}
	Texture(Film* aImg){
		mImg = aImg;
	}
private:
	int mod(int x, int y){
		x %= y;
		if (x < 0)
			x += y;
		return x;
	}
	Film* mImg;
	int mType;
};

}; // namespace Raytrace

#endif // CORE_TEXTURE_H
