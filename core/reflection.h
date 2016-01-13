// core/reflection.h
#ifndef CORE_REFLECTION_H
#define CORE_REFLECTION_H

#include "common.h"

namespace Raytracer {

inline Vec3f reflect(Vec3f incident, Vec3f norm){
	return incident - 2.0f * dot(incident, norm) * norm;
}

class Fresnel{
public:
	Fresnel(real eta = 1.0f) : eta(eta){}
	virtual real getReflection(real cosI){
		real n = cosI > 0 ? eta : 1.0 / eta;
		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
		if (cosT2 > 0.0f){
			real cosT = sqrt(cosT2);
			real r_p = (n * cosI - cosT) / (n * cosI + cosT);
			real r_v = (cosI - n * cosT) / (cosI + n * cosT);
			return (r_p * r_p + r_v * r_v) / 2.0;
		}else
			return 1.0f;
	}
	Vec3f refract(Vec3f incident, Vec3f norm, real cosI){
		real n = cosI > 0 ? eta : 1.0 / eta;
		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
		return (n * incident) - (n * cosI + sqrt( cosT2 )) * norm;
	}
private:
	real eta;
};

class NoFresnel : public Fresnel{
public:
	real getReflection(real cosI){
		return 1.0f;
	}
};

}; // namespace Raytracer

#endif // CORE_REFLECTION_H
