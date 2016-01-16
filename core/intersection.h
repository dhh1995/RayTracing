// core/intersection.h
#ifndef CORE_INTERSECTION_H
#define CORE_INTERSECTION_H

#include "common.h"
#include "primitive.h"

namespace Raytracer {

class Intersection{
public:
	Intersection(Primitive* aPrim = NULL, Vec3f aPos = ORIGINAL, real aDist = INF, bool aBack = false)
		: mPrim(aPrim), mPos(aPos), mDist(aDist){
		mLight = false;
	}
	
	void setPos(Vec3f aPos){ mPos = aPos; }
	Vec3f getPos() const{ return mPos; }

	void setNorm(Vec3f aNorm){ mNorm = aNorm; }
	Vec3f getNorm() const{ return mNorm; }

	void setDist(real aDist){ mDist = aDist; }
	real getDist() const{ return mDist; }

	void setPrim(Primitive* aPrim){ mPrim = aPrim; }
	Primitive* getPrim() const{ return mPrim; }

	void setLight(bool flag = true){ mLight = flag; }
	bool isLight(){ return mLight; }
	
	// void setUV(const UV &uv){ mUV = uv; }
	// UV getUV(){ return mUV; }
private:
	bool mLight;
	real mDist;
	Vec3f mPos;
	Vec3f mNorm;
	Primitive* mPrim;
	// UV mUV;
};

}; // namespace Raytracer

#endif // CORE_INTERSECTION_H
