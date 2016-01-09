// renderers/testrenderer.h
#ifndef CORE_TESTRENDER_H
#define CORE_TESTRENDER_H

#include "core/common.h"
#include "core/renderer.h"
#include "core/intersection.h"
#include "core/primitive.h"
#include "core/sampler.h"


namespace Raytracer {

const int TRACEDEPTH = 8;

class TestRenderer : public Renderer {
public:
	TestRenderer():mRaysCast(0){
	}
	void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist);
	void render(const Args& args);
	void show();
	//Options options;
protected:
	real _FresnelReflection(real n, real cosI, real cosT2);
	Vec3f _getRandomDir(){
		real x, y, z;
		do{
			x = Sampler::getRandReal() * 2 - 1;
			y = Sampler::getRandReal() * 2 - 1;
			z = Sampler::getRandReal() * 2 - 1;
			//printf("%lf %lf %lf\n", x, y, z);
		}while (x*x + y*y + z*z > 1);
		return Vec3f(x, y, z).Normalize();
	}
	Vec3f _getDiffuseDir(Vec3f N){
		Vec3f dir;
		do
			dir = _getRandomDir();
		while (dot(N, dir) < 0);
	  return dir;
	}

	int mRaysCast;
};

}; // namespace Raytracer

#endif // CORE_TESTRENDER_H
