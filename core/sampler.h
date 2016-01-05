// core/sampler.h
#ifndef CORE_SAMPLER_H
#define CORE_SAMPLER_H

#include "common.h"

namespace Raytracer {
	
class Sampler{
public:
	//Sampler();
	//virtual void sample();
	static real getRand(){
		return rand() / RAND_MAX;
	}
//private:
};

}; // namespace Raytracer

#endif // CORE_SAMPLER_H
