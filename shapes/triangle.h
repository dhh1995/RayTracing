// shapes/triangle.h
#ifndef SHAPES_TRIANGLE_H
#define SHAPES_TRIANGLE_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"
#include "core/vertex.h"
#include "shapes/box.h"

namespace Raytracer {

class Triangle : public Primitive{
public:
	string getType(){
		return "Triangle";
	}
	Triangle(Vertex* A, Vertex* B, Vertex* C):A(A), B(B), C(C){
		a = b = c = -1;
		_computeNorm();
	}
	Triangle(const vector<Vertex*> vex, int a, int b, int c):a(a), b(b), c(c), A(vex[a]), B(vex[b]), C(vex[c]){
		_computeNorm();
	}
	int getVexID(int i){
		return i == 0 ? a : i == 1 ? b : c;
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
	void _computeNorm(){
		//mV = (B - A).Normalize();
		//mU = (C - A).Normalize();
		mNorm = cross(B->getPos() - A->getPos(), C->getPos() - A->getPos()).Normalize();
		mD = - dot(A->getPos(), mNorm);
	}
	int a, b, c;
	Vertex* A;
	Vertex* B;
	Vertex* C;
	Vec3f mNorm; //, mU, mV;
	real mD;
//private:
};

}; // namespace Raytrace


#endif // SHAPES_TRIANGLE_H
