#include "reflection.h"

namespace Raytracer {

Color BxDF::sampleF(const Vec3f &wo, Vec3f& wi, real& pdf, BxDFType& sampledType) const {
	// Cosine-sample the hemisphere, flipping the direction if necessary
	wi = Sampler::cosSampleHemisphere();
	if (wo.z < 0)
		wi.z *= -1;
	pdf = Pdf(wo, wi);
	return f(wo, wi);
}

real BxDF::Pdf(const Vec3f &wo, const Vec3f &wi) const {
	return wo.z * wi.z > 0 ? absCosTheta(wi) / PI : 0;
}

Color BSDF::f(const Vec3f &wo, const Vec3f &wi, BxDFType flags){
	bool reflect = true;
	Color f;
	for (BxDF* bxdf : bxdfs)
		if (bxdf->matchesFlags(flags) &&
			((reflect && (bxdf->type & BSDF_REFLECTION)) ||
			(!reflect && (bxdf->type & BSDF_TRANSMISSION))))
			f += bxdf->f(wo, wi);
	return f;
}

}; // namespace Raytracer
