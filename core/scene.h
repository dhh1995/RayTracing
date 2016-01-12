// core/scene.h
#ifndef CORE_SCENE_H
#define CORE_SCENE_H

#include "common.h"
#include "light.h"
#include "primitive.h"
#include "intersection.h"
#include "kdtree.h"
#include "shapes/trianglemesh.h"
#include "shapes/box.h"

namespace Raytracer {

class Scene{
public:
	Scene(Color aAmbient = BLACK): mAmbient(aAmbient){
		mAggregate.clear();
		mPrimitives.clear();
		mLights.clear();
	}
	bool intersect(const Ray& ray, Intersection& isect, bool useLight = false);
	bool intersectP(const Ray& ray);
	void addLight(Light *aLight){
		mLights.push_back(aLight);
	}
	vector<Light* > getLights(){
		return mLights;
	}
	void addObject(Primitive *aObject){
		if (aObject->getType() == "TriangleMesh"){
			TriangleMesh* triMesh = static_cast<TriangleMesh*>(aObject);
			for (Triangle* tri : triMesh->getTriangles())
				mAggregate.add(tri);
		}else
			mPrimitives.push_back(aObject);
	}
	void construct(bool debug = false){
		KdTreeTri::debug = debug;
		mAggregate.construct();
	}
	real visible(const Ray& ray, real dist);
	// real calcShade(Light* light, Vec3f pos, Vec3f& dir);
	// Color getLi(const Ray& ray, const Intersection& isect);
	bool loadObj(string path);
private:
	int chg(int n, int x){
		if (x < 0)
			return n + x - 1;
		return x - 1;
	}
	//Primitive* mAggregate;
	vector<Primitive* > mPrimitives;
	KdTreeTri mAggregate;
	vector<Light* > mLights;
	Color mAmbient;
};

}; // namespace Raytracer

#endif // CORE_SCENE_H
