// renderers/testrenderer.h
#ifndef CORE_TESTRENDER_H
#define CORE_TESTRENDER_H

#include "core/common.h"
#include "core/renderer.h"
#include "core/intersection.h"
#include "core/primitive.h"
#include "core/sampler.h"


namespace Raytracer {

const int TRACEDEPTH = 5;

class TestRenderer : public Renderer {
public:
	TestRenderer():mRaysCast(0){
	}
	Color directLight(const Ray& ray, const Intersection& isect);
	void rayTracing(Ray ray, Color& res, int depth);
	void render(const Args& args);
	void show(bool wait = false){
		mCamera->getFilm()->show(wait);
	}
protected:
	void _showImage(Film* film, int w, int h, Color* resultColor, int* counter, bool tem = true);
	int mRaysCast;
};

}; // namespace Raytracer

#endif // CORE_TESTRENDER_H
