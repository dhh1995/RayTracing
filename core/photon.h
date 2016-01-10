// core/photon.h
#ifndef CORE_PHOTON_H
#define CORE_PHOTON_H

#include "common.h"

namespace Raytracer {

class Photon{
public:
	Photon():mRay(ORIGINAL, ZERO){}
	Photon(Ray aRay, Color aPower) : mRay(aRay), mPower(aPower){
	}
	Ray& getRay(){
		return mRay;
	}
	void setRay(Ray aRay){
		mRay = aRay;
	}
	Vec3f getPos() const{
		return mRay.o;
	}
	real getAxis(int dim) const{
		return mRay.o[dim];
	}
	Color getPower(){
		return mPower;
	}
	void updatePower(Color upd){
		mPower *= upd;
	}
	void prt(){
		mRay.prt();
	}
	void dump(FILE* fp){
		fprintf(fp, "%lf %lf %lf\n",mRay.o.x, mRay.o.y, mRay.o.z);
		fprintf(fp, "%lf %lf %lf\n",mRay.d.x, mRay.d.y, mRay.d.z);
		fprintf(fp, "%lf %lf %lf\n",mPower.x, mPower.y, mPower.z);
	}
	void load(FILE* fp){
		real x, y, z;
		fscanf(fp, "%lf%lf%lf", &x, &y, &z);
		Vec3f o(x,y,z);
		fscanf(fp, "%lf%lf%lf", &x, &y, &z);
		Vec3f d(x,y,z);
		mRay = Ray(o, d);
		fscanf(fp, "%lf%lf%lf", &x, &y, &z);
		mPower = Color(x, y, z);
	}
private:
	Ray mRay;
	Color mPower;

};



}; // namespace Raytracer

#endif // CORE_PHOTON_H
