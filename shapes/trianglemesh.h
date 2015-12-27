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
	Box getBBox(){
		Box box(A);
		box.update(B);
		box.update(C);
		return box;
	}
	int getSide(int dim, real split){
		int cnt[2] = {0, 0};
		split -= EPS;
		++ cnt[A[dim] < split];
		++ cnt[B[dim] < split];
		++ cnt[C[dim] < split];
		if (cnt[0] == 3)
			return -1;
		if (cnt[1] == 3)
			return 1;
		return 0;
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool inside(const Vec3f pos);
	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray);
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
	vector<Triangle*> getTriangles(){
		return mTriangles;
	}
	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray){}
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
