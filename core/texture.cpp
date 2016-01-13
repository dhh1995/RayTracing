#include "texture.h"

namespace Raytracer {

Color Texture::getColor(UV uv){
	real u = uv.first, v = uv.second;
	// fetch a bilinearly filtered texel
	real fu = u * mUScale;
	real fv = v * mVScale;
	int u1 = ((int)fu) % mImg->getW();
	if (u1 < 0)
		u1 += mImg->getW();
	int v1 = ((int)fv) % mImg->getH();
	if (v1 < 0)
		v1 += mImg->getH();
	int u2 = (u1 + 1) % mImg->getW();
	int v2 = (v1 + 1) % mImg->getH();
	// calculate fractional parts of u and v
	real fracu = fu - floor( fu );
	real fracv = fv - floor( fv );
	// calculate weight factors
	real w1 = (1 - fracu) * (1 - fracv);
	real w2 = fracu * (1 - fracv);
	real w3 = (1 - fracu) * fracv;
	real w4 = fracu *  fracv;
	// fetch four texels
	Color c1 = mImg->getColor(u1, v1);
	Color c2 = mImg->getColor(u2, v1);
	Color c3 = mImg->getColor(u1, v2);
	Color c4 = mImg->getColor(u2, v2);
	// scale and sum the four colors
	return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
}

}; // namespace Raytracer
