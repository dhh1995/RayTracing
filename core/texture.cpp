#include "texture.h"

namespace Raytracer {

Color Texture::getColor(Vec3f pos){
	real fu = (dot((pos - mO), mU) * mScaleU);
	real fv = (dot((pos - mO), mV) * mScaleV);

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

Color CheckerBoard::getColor(Vec3f pos){
	real fu = (dot((pos - mO), mU) * mScaleU);
	real fv = (dot((pos - mO), mV) * mScaleV);
	int w = 100, h = 100;
	int u1 = ((int)fu) % w;
	if (u1 < 0)
		u1 += w;
	int v1 = ((int)fv) % h;
	if (v1 < 0)
		v1 += h;
	int u2 = (u1 + 1) % w;
	int v2 = (v1 + 1) % h;
	// calculate fractional parts of u and v
	real fracu = fu - floor( fu );
	real fracv = fv - floor( fv );
	// calculate weight factors
	real w1 = (1 - fracu) * (1 - fracv);
	real w2 = fracu * (1 - fracv);
	real w3 = (1 - fracu) * fracv;
	real w4 = fracu *  fracv;
	// fetch four texels
	Color c1 = _getColor(u1, v1);
	Color c2 = _getColor(u2, v1);
	Color c3 = _getColor(u1, v2);
	Color c4 = _getColor(u2, v2);
	// scale and sum the four colors
	return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
}

Vec3f BumpTexture::getNorm(Vec3f pos, Vec3f norm){
	int w = mImg->getW(), h = mImg->getH();
	int uu = mod((int)(dot((pos - mO), mU) * mScaleU), w);
	int vv = mod((int)(dot((pos - mO), mV) * mScaleV), h);

	int u1 = uu + 1;
	if (u1 >= w) u1 -= w;
	int u2 = uu - 1;
	if (u2 < 0) u2 += w;
	Color du = mImg->getColor(u1, vv) - mImg->getColor(u2, vv);

	int v1 = vv + 1;
	if (v1 >= h) v1 -= h;
	int v2 = vv - 1;
	if (v2 < 0) v2 += h;
	Color dv = mImg->getColor(uu, v1) - mImg->getColor(uu, v2);

	return (norm + du.getMean() * mU * mRate + dv.getMean() * mV * mRate).Normalize();
}

}; // namespace Raytracer
