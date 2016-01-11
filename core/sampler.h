// core/sampler.h
#ifndef CORE_SAMPLER_H
#define CORE_SAMPLER_H

#include "common.h"

namespace Raytracer {

//P720

class Sampler{
public:
	//Sampler();
	//virtual void sample();
	static int getRandInt(int x){
		return rand() % x;
	}
	static real getRandReal(){
		return rand() / real(RAND_MAX);
	}
	static Vec3f getRandomDir(){
		real x, y, z;
		do{
			x = getRandReal() * 2 - 1;
			y = getRandReal() * 2 - 1;
			z = getRandReal() * 2 - 1;
		}while (x*x + y*y + z*z > 1);
		return Vec3f(x, y, z).Normalize();
	}
	static void concentricSampleDisk(real& x, real& y);
	static Vec3f cosSampleHemisphere() {
		real x, y, z;
		concentricSampleDisk(x, y);
		z = sqrt(max(0.0, 1 - x * x - y * y));
		return Vec3f(x, y, z);
	}
//private:
};

}; // namespace Raytracer

#endif // CORE_SAMPLER_H
