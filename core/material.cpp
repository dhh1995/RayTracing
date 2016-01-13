#include "material.h"

namespace Raytracer {

Color Material::sample(const Vec3f& wi, Vec3f &wo, Vec3f norm, Vec3f pos, real& pdf){
	real Pd = mDiff.getMax();
	real Ps = mSpec.getMax();
	real ran = Sampler::getRandReal();
	pdf = 1.0f;
	if (ran * (Ps + Pd) < Ps){
		ran *= (Ps + Pd);
		real cosI = dot(wi, norm);
		real refl = mFresnel->getReflection(cosI);
		if (ran < Ps * refl)
			wo = reflect(wi, norm);
		else
			wo = mFresnel->refract(wi, norm, cosI);
		return mSpec; // Ps * (Pd + Ps);
	}else{
		wo = Sampler::getDiffuseDir(norm);
		return mDiff; // Pd * (Pd + Ps);
	}
}

}; // namespace Raytracer
