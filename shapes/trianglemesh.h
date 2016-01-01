// shapes/trianglemesh.h
#ifndef SHAPES_TRIANGLEMESH_H
#define SHAPES_TRIANGLEMESH_H

#include "core/common.h"
#include "core/primitive.h"
#include "core/intersection.h"
#include "core/meshvertex.h"
#include "core/kdtree.h"
#include "shapes/triangle.h"
#include "shapes/box.h"

namespace Raytracer {

class TriangleMesh : public Primitive{
public:
	TriangleMesh(string objFile, Material* aMaterial, Vec3f trans = ORIGINAL, real scale = 1.0f);
	string getType(){
		return "TriangleMesh";
	}
	vector<Triangle*> getTriangles(){
		return mTriangles;
	}
	//HomeWork 2 Mesh decimation.
  	void contraction(VertexPair P);
	void decimation(real percent, real threshold = 0);

	bool intersect(const Ray& ray, Intersection& isect); //brute force, use kd-tree for whole scene instead.
	bool intersectP(const Ray& ray){}
	Vec3f getNorm(Vec3f pos){}
	~TriangleMesh();
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

	priority_queue<VertexPair> Q;
	VertexCloud mVexCloud;
};


}; // namespace Raytrace


#endif // SHAPES_TRIANGLEMESH_H
