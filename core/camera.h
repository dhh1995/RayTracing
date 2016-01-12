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
	virtual void setLookAt(Vec3f lookAt, Vec3f aI) = 0;
	virtual Ray sampleRay(int x, int y) = 0;
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
			l = tan(- mArc / 2), r = tan(mArc / 2);
			printf("camera left = %lf right = %lf\n",l,r);
	}
	void convert(int x, int y, real &dx, real &dy);
	void setLookAt(Vec3f lookAt, Vec3f aI){
		mLookAt = lookAt;
		mI = aI;
		mJ = cross(mI, mLookAt);
	}
	Ray sampleRay(int x, int y);
protected:
	//J x I = LookAt
	Vec3f mLookAt;
	Vec3f mI, mJ;
	real mFov, mArc;
	real l, r;
};

class PerspectiveCamera : public ProjectiveCamera{
public:
	PerspectiveCamera(Vec3f aPos, Vec3f aLookAt, Vec3f aI, real aFov, real aLensRadius, real aFocalDist)
		: ProjectiveCamera(aPos, aLookAt, aI, aFov), mLensRadius(aLensRadius), mFocalDist(aFocalDist){
	}
	Ray sampleRay(int x, int y);
private:
	real mLensRadius, mFocalDist;
};

}; // namespace Raytracer
 
#endif // CORE_CAMERA_H
