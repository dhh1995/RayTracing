// renderers/testrenderer.h
#ifndef CORE_TESTRENDER_H
#define CORE_TESTRENDER_H

#include "core/common.h"
#include "core/renderer.h"
#include "core/intersection.h"
#include "core/primitive.h"
#include "core/sampler.h"
#include "core/reflection.h"


namespace Raytracer {

const int TRACEDEPTH = 12;

class TestRenderer : public Renderer {
public:
	TestRenderer():mRaysCast(0){
	}
	void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist, int needSamples);
	void render(const Args& args);
	void show(bool wait = false){
		mCamera->getFilm()->show(wait);
	}
	Color directLight(const Ray& ray, Vec3f hitPoint, BSDF* bsdf);
protected:
	real _FresnelReflection(real n, real cosI, real cosT2);
	Vec3f _getDiffuseDir(Vec3f N){
		Vec3f dir;
		do
			dir = Sampler::getRandomDir();
		while (dot(N, dir) < 0);
		return dir;
	}

	int mRaysCast;
};

}; // namespace Raytracer

#endif // CORE_TESTRENDER_H
