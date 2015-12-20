// shapes/trianglemesh.h
#ifndef SHAPES_TRIANGLEMESH_H
#define SHAPES_TRIANGLEMESH_H

#include "core/common.h"
#include "core/shape.h"
#include "core/intersection.h"

namespace Raytracer {

class Triangle : public Shape{
public:
	string getType(){
		return "Triangle";
	}
	Triangle(Vec3f A, Vec3f B, Vec3f C):A(A), B(B), C(C){
		E1 = A - B, E2 = A - C;
		mNorm = cross(E1, E2);
	}
	int intersect(const Ray& ray, Intersection& isect);
	int intersectP(const Ray& ray);
	Vec3f getNorm(Vec3f pos){
		return mNorm;
	}
	Vec3f A, B, C, E1, E2, mNorm;
	real mD;
//private:
};

class TriangleMesh : public Shape{
public:
	TriangleMesh(string objFile, Vec3f trans);
	string getType(){
		return "TriangleMesh";
	}
	int intersect(const Ray& ray, Intersection& isect);
	int intersectP(const Ray& ray){}
	Vec3f getNorm(Vec3f pos){}
private:
	int chg(int x){
		if (x < 0)
			return mVertexs.size() + x - 1;
		return x - 1;
	}
	//friend class Triangle;
	vector<Vec3f> mVertexs;
	vector<Triangle*> mTriangles;
};


}; // namespace Raytrace


#endif // SHAPES_TRIANGLEMESH_H
