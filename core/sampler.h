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
	static void unitDisk(real* x, real* y);
//private:
};

}; // namespace Raytracer

#endif // CORE_SAMPLER_H
