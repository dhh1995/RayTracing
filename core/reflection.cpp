#include "reflection.h"

namespace Raytracer {

Color BxDF::sampleF(const Vec3f &wo, Vec3f& wi, real& pdf, BxDFType& type) const {
	wi = Sampler::cosSampleHemisphere();
	if (wo.z < 0)
		wi.z *= -1;
	pdf = Pdf(wo, wi);
	return f(wo, wi);
}

real BxDF::Pdf(const Vec3f &wo, const Vec3f &wi) const {
	return wo.z * wi.z > 0 ? absCosTheta(wi) / PI : 0;
}

Color BSDF::f(const Vec3f &woWorld, const Vec3f &wiWorld, BxDFType flags){
	Vec3f wo = toLocal(woWorld), wi = toLocal(wiWorld);
	bool reflect = true;
	Color f;
	for (BxDF* bxdf : bxdfs)
		if (bxdf->matchesFlags(flags) &&
			((reflect && (bxdf->type & BSDF_REFLECTION)) ||
			(!reflect && (bxdf->type & BSDF_TRANSMISSION))))
			f += bxdf->f(wo, wi);
	return f;
}

Color BSDF::sampleF(const Vec3f &woWorld, Vec3f& wiWorld, real& pdf, BxDFType& type) const{
	Vec3f wo = toLocal(woWorld), wi;
	int nMatch = numComponents(type);
	int index = Sampler::getRandInt(nMatch);
	BxDF* sampleBxDF = NULL;
	for (BxDF* bxdf : bxdfs)
		if (bxdf->matchesFlags(type))
			if (index-- == 0){
				sampleBxDF = bxdf;
				break;
			}
	Color res = sampleBxDF->sampleF(wo, wi, pdf, type);
	for (BxDF* bxdf : bxdfs)
		if (bxdf != sampleBxDF)
			res += bxdf->f(wo, wi);
	wiWorld = toWorld(wi);
	return res / int(bxdfs.size());
}

}; // namespace Raytracer
