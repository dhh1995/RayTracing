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
	static Vec3f randVector(){
		return Vec3f(
			getRandReal() * 2 - 1,
			getRandReal() * 2 - 1,
			getRandReal() * 2 - 1);
	}
	static Vec3f getRandomDir(){ //random dir in sphere
		Vec3f dir;
		do
			dir = randVector();
		while (dir.L2() > 1);
		return dir.Normalize();
	}
	static void concentricSampleDisk(real& x, real& y); //map to (r, theta) -> x = r cos(theta), y = r sin(theta)
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
