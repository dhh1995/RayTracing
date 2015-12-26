// core/camera.h
#ifndef CORE_CAMERA_H
#define CORE_CAMERA_H

#include "common.h"
#include "film.h"

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
	virtual vector<Ray> generateRays() = 0;
	virtual ~Camera(){
		delete(mFilm);
	}
public:
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
	vector<Ray> generateRays();
private:
	//J x I = LookAt
	Vec3f mLookAt;
	Vec3f mI, mJ;
	real mFov, mArc;
	//real mLensRadius, mFocalDistance;
};

}; // namespace Raytracer
 
#endif // CORE_CAMERA_H
