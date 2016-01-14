#include "material.h"

namespace Raytracer {

Color Material::sample(const Vec3f& wi, Vec3f &wo, Vec3f norm, Vec3f pos, real& pdf, bool& isSpecular){
	real Pd = mDiff.getMean();
	real Ps = mSpec.getMean();
	real ran = Sampler::getRandReal();
	isSpecular = false;
	pdf = 1.0f;
	if (ran * (Ps + Pd) < Ps){
		ran *= (Ps + Pd);
		real cosI = dot(wi, norm);
		real refl = mFresnel->getReflection(cosI);
		if (ran < Ps * refl)
			wo = reflect(wi, norm);
		else
			wo = mFresnel->refract(wi, norm, cosI);
		isSpecular = true;
		return mSpec; // Ps * (Pd + Ps);
	}else{
		wo = Sampler::getDiffuseDir(norm);
		return mDiff; // Pd * (Pd + Ps);
	}
}

}; // namespace Raytracer
