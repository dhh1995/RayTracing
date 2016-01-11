// core/reflection.h
#ifndef CORE_REFLECTION_H
#define CORE_REFLECTION_H

#include "common.h"
#include "sampler.h"

namespace Raytracer {

inline real absCosTheta(const Vec3f &w) { return abs(w.z); }

inline Vec3f reflect(const Vec3f& wo, const Vec3f& norm){
	return 2 * dot(wo, norm) * norm - wo;
}

inline bool refract(const Vec3f& wi, Vec3f& wt, const Vec3f& norm, real eta){
	real cosI = dot(norm, wi);
	real cosT2 = 1.0f - eta * eta * (1.0f - cosI * cosI);
	if (cosT2 < 0.0f) return false;
	wt = (eta * -wi) + (eta * cosI - sqrt( cosT2 )) * norm;
	return true;
}

enum BxDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
			   BSDF_TRANSMISSION,
};

class BxDF{
public:
	BxDF(BxDFType type) : type(type) {}
	bool matchesFlags(BxDFType t) const {
		return (type & t) == type;
	}
	virtual Color f(const Vec3f &wo, const Vec3f &wi) const = 0;
	virtual Color sampleF(const Vec3f &wo, Vec3f& wi, real& pdf, BxDFType& sampledType) const;
	virtual real Pdf(const Vec3f &wi, const Vec3f &wo) const;
	const BxDFType type;
};

class LambertianReflection : public BxDF{
public:
	LambertianReflection(const Color &R)
		: BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) {}
	Color f(const Vec3f &wo, const Vec3f &wi) const { return R / PI; }
private:
	const Color R;
};

class LambertianTransmission : public BxDF{
public:
	LambertianTransmission(const Color &T)
		: BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_DIFFUSE)), T(T) {}
	Color f(const Vec3f &wo, const Vec3f &wi) const { return T / PI; }
private:
	const Color T;
};

class SpecularReflection : public BxDF{
public:
	SpecularReflection(const Color &R)
		: BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), R(R) {}
	Color f(const Vec3f &wo, const Vec3f &wi) const { return BLACK; } 
	Color sampleF(const Vec3f &wo, Vec3f& wi, real& pdf, BxDFType& sampledType) const{
		wi = Vec3f(-wo.x, -wo.y, wo.z);
		pdf = 1.0;
		return R / absCosTheta(wi);
	}
	real Pdf(const Vec3f &wi, const Vec3f &wo) const { return 0; }
private:
	const Color R;
};

class SpecularTransmission : public BxDF{
public:
	SpecularTransmission(const Color &T)
		: BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR)), T(T) {}
	Color f(const Vec3f &wo, const Vec3f &wi) const { return BLACK; } 
	Color sampleF(const Vec3f &wo, Vec3f& wi, real& pdf, BxDFType& sampledType) const{
		// if (!refract(wo, wi))
		// 	return BLACK;
		// pdf = 1.0;
		// return R / absCosTheta(wi);
	}
	real Pdf(const Vec3f &wi, const Vec3f &wo){ return 0; }
private:
	const Color T;
};

class BSDF{
public:
	int numComponents(BxDFType flags = BSDF_ALL) const{
		int num = 0;
		for (BxDF* bxdf : bxdfs)
			if (bxdf->matchesFlags(flags)) ++num;
		return num;
	}
	void addBxDF(BxDF* bxdf){
		bxdfs.push_back(bxdf);
	}
	Color f(const Vec3f &wo, const Vec3f &wi, BxDFType flags = BSDF_ALL);
private:
	vector<BxDF* > bxdfs;
};

}; // namespace Raytracer

#endif // CORE_REFLECTION_H
