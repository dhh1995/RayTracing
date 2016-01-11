// core/camera.h
#ifndef CORE_CAMERA_H
#define CORE_CAMERA_H

#include "common.h"
#include "film.h"
#include "sampler.h"
#include "args.h"

namespace Raytracer {

class Camera{
public:
	Camera(Vec3f aPos) : mPos(aPos){
		mFilm = NULL;
	}
	Film* getFilm(){
		return mFilm;
	}
	void setFilm(Film* aFilm){
		mFilm = aFilm;
	}
	Vec3f getPos(){
		return mPos;
	}
	void setPos(Vec3f aPos){
		mPos = aPos;
	}
	virtual Ray generateRay(real dx, real dy, int i, int j) = 0;
	virtual vector<Ray> generateRays(const Args& args) = 0;
	virtual ~Camera(){
		delete(mFilm);
	}
protected:
	Vec3f mPos;
private:
	Film* mFilm;
};

class ProjectiveCamera : public Camera{
public:
	ProjectiveCamera(Vec3f aPos, Vec3f aLookAt, Vec3f aI, real aFov)
		: Camera(aPos), mLookAt(aLookAt), mI(aI), mFov(aFov){
			mJ = cross(mI, mLookAt);
			mArc  = mFov / 180 * PI;
	}
	Ray generateRay(real dx, real dy, int i, int j);
	vector<Ray> generateRays(const Args& args);
protected:
	//J x I = LookAt
	Vec3f mLookAt;
	Vec3f mI, mJ;
	real mFov, mArc;
};

class PerspectiveCamera : public ProjectiveCamera{
public:
	PerspectiveCamera(Vec3f aPos, Vec3f aLookAt, Vec3f aI, real aFov, real aLensRadius, real aFocalDist)
		: ProjectiveCamera(aPos, aLookAt, aI, aFov), mLensRadius(aLensRadius), mFocalDist(aFocalDist){
	}
	Ray generateRay(real dx, real dy, int i, int j);
	vector<Ray> generateRays(const Args& args);
private:
	real mLensRadius, mFocalDist;
};

}; // namespace Raytracer
 
#endif // CORE_CAMERA_H
