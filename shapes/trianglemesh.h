// shapes/trianglemesh.h
#ifndef SHAPES_TRIANGLEMESH_H
#define SHAPES_TRIANGLEMESH_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"
#include "shapes/box.h"

namespace Raytracer {

class Vertex{
public:
	Vertex(Vec3f aPos):mPos(aPos){
	}
	Vec3f getPos(){
		return mPos;
	}
	void prt(){
		printf("pos = ");
		mPos.prt();
		printf("norm = ");
		mNorm.prt();
	}
private:
	Vec3f mPos; // pos
	Vec3f mNorm; // norm
};

class Triangle : public Primitive{
public:
	string getType(){
		return "Triangle";
	}
	Triangle(Vertex* A, Vertex* B, Vertex* C):A(A), B(B), C(C){
		//mV = (B - A).Normalize();
		//mU = (C - A).Normalize();
		mNorm = cross(B->getPos() - A->getPos(), C->getPos() - A->getPos()).Normalize();
		mD = - dot(A->getPos(), mNorm);
	}
	void prt(){
		colorMessage("Triangle:", 3);
		A->prt();
		B->prt();
		C->prt();
	}
	Box getBBox(){
		Box box(A->getPos());
		box.update(B->getPos());
		box.update(C->getPos());
		return box;
	}
	int getSide(int dim, real split){
		int cnt[2] = {0, 0};
		split -= EPS;
		++ cnt[A->getPos()[dim] < split];
		++ cnt[B->getPos()[dim] < split];
		++ cnt[C->getPos()[dim] < split];
		if (cnt[1] == 3)
			return -1;
		if (cnt[0] == 3)
			return 1;
		return 0;
	}
	void setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide);
	bool inside(const Vec3f pos);
	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray);
private:
	Vertex* A;
	Vertex* B;
	Vertex* C;
	Vec3f mNorm; //, mU, mV;
	real mD;
//private:
};

class TriangleMesh : public Primitive{
public:
	TriangleMesh(string objFile, Material* aMaterial, Vec3f trans = ORIGINAL, real scale = 1.0f);
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
	vector<Vertex* > mVertexs;
	vector<Triangle*> mTriangles;
	Box* mBoundingBox;
};


}; // namespace Raytrace


#endif // SHAPES_TRIANGLEMESH_H
