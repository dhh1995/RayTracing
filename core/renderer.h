// core/renderer.h
#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "common.h"
#include "camera.h"
#include "scene.h"
#include "args.h"

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
	virtual void render(const Args& args) = 0;
	virtual void show() = 0;
protected:
	Camera* mCamera;
	Scene* mScene;
//private:
};

}; // namespace Raytracer

#endif // CORE_RENDERER_H
