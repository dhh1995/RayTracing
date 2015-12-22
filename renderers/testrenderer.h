// renderers/testrenderer.h
#ifndef CORE_TESTRENDER_H
#define CORE_TESTRENDER_H

#include "core/common.h"
#include "core/geometry.h"
#include "core/renderer.h"
#include "core/camera.h"
#include "core/scene.h"
#include "core/intersection.h"
#include "core/primitive.h"


namespace Raytracer {

class TestRenderer : public Renderer {
public:
	TestRenderer():mRaysCast(0){
	}
	void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist);
	void render();
	void show();
	//Options options;
private:
	int mRaysCast;
};

}; // namespace Raytracer

#endif // CORE_TESTRENDER_H
