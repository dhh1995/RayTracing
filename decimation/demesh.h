// shapes/trianglemesh.h
#ifndef DECIMATION_DEMESH_H
#define DECIMATION_DEMESH_H

#include "core/common.h"
#include "core/kdtree.h"
#include "decimation/devertex.h"
#include "shapes/trianglemesh.h"

using namespace Raytracer;

namespace Decimation {

class DeMesh : public TriangleMesh {
public:
	DeMesh(string objFile, Material* aMaterial, Vec3f trans = ORIGINAL, real scale = 1.0f)
		: TriangleMesh(objFile, aMaterial, trans, scale){
	}
	void initialize(){
		//convert to Decimation data, except for pointers in Triangle are Vertex, not DeVertex.
		int n = 0;
		mVexCloud.clear();
		for (Vertex* vex : mVertexs)
			mVexCloud.add(new DeVertex(*vex, n ++));
		merged = new bool[n];
		mDeTriangles.clear();
		for (Triangle* tri : mTriangles)
			mDeTriangles.push_back(new DeTriangle(*tri, mVexCloud.getData()));
		mVexCloud.construct();
	}
	string getType(){
		return "DecimationMesh";
	}
  	int contraction(VertexPair P);
	void decimation(real percent, real threshold = 0);
	~DeMesh(){
		delete[] merged;
	}
private:
	bool _checkValid(VertexPair P){
		if (merged[P.A->getID()])
			return false;
		if (merged[P.B->getID()])
			return false;
		return true;
	}
	//void _filter(vector<DeTriangle*> &triangles);
	priority_queue<VertexPair> Q;
	KdTree<DeVertex> mVexCloud;
	vector<DeTriangle* > mDeTriangles;
	bool* merged;
};

}; // namespace Decimation


#endif // DECIMATION_DEMESH_H
