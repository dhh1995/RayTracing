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
	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray);
	void addLight(Light *aLight){
		mLights.push_back(aLight);
	}
	void addObject(Primitive *aObject){
		if (aObject->getType() == "233TriangleMesh"){
			TriangleMesh* triMesh = dynamic_cast<TriangleMesh*>(aObject);
			for (Triangle* tri : triMesh->getTriangles())
				mAggregate.add(tri);
		}else
			mPrimitives.push_back(aObject);
	}
	void construct(){
		KdTreeTri::debug = false;
		mAggregate.construct();
	}
	real calcShade(Light* light, Vec3f pos, Vec3f& dir);
	Color getLi(const Ray& ray, const Intersection& isect);
	void loadObj();//TODO
private:
	//Primitive* mAggregate;
	vector<Primitive* > mPrimitives;
	KdTreeTri mAggregate;
	vector<Light* > mLights;
	Color mAmbient;
};

}; // namespace Raytrace

#endif // CORE_SCENE_H
