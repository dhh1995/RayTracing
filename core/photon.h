// core/photon.h
#ifndef CORE_PHOTON_H
#define CORE_PHOTON_H

#include "common.h"

namespace Raytracer {

class Photon{
public:
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
	// void read(){
	// 	mPos.read();
	// 	mPower.read();
	// }
	// void prt(){
	// 	mPos.prt();
	// 	mPower.prt());
	// }
private:
	Ray mRay;
	Color mPower;

};



}; // namespace Raytracer

#endif // CORE_PHOTON_H
