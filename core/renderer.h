// core/renderer.h
#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "common.h"

namespace Raytracer {

class Renderer{
public:
	Renderer() : mCamera(NULL), mScene(NULL){
	}
	void setScene(Scene* aScene){
		mScene = aScene;
	}
	void setCamera(Camera* aCamera){
		mCamera = aCamera;
	}
	Scene* getScene(){
		return mScene;
	}
	Camera* getCamera(){
		return mCamera;
	}

	virtual void rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist) = 0; // for debug
	virtual void render() = 0;
	virtual void show() = 0;
	Camera* mCamera;
	Scene* mScene;
//private:
};

}; // namespace Raytrace

#endif // CORE_RENDERER_H
