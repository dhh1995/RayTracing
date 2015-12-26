// shapes/trianglemesh.h
#ifndef SHAPES_TRIANGLEMESH_H
#define SHAPES_TRIANGLEMESH_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"
#include "shapes/box.h"

namespace Raytracer {

class Triangle : public Primitive{
public:
	string getType(){
		return "Triangle";
	}
	Triangle(Vec3f A, Vec3f B, Vec3f C):A(A), B(B), C(C){
		//mV = (B - A).Normalize();
		//mU = (C - A).Normalize();
		mNorm = cross(B - A, C - A).Normalize();
		mD = - dot(A, mNorm);
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool inside(const Vec3f pos);
	int intersect(const Ray& ray, Intersection& isect);
	int intersectP(const Ray& ray);
	Vec3f A, B, C, mNorm; //, mU, mV;
	real mD;
//private:
};

class TriangleMesh : public Primitive{
public:
	TriangleMesh(string objFile, Material* aMaterial, Vec3f trans = ORIGINAL);
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
	Box* mBoundingBox;
};


}; // namespace Raytrace


#endif // SHAPES_TRIANGLEMESH_H
