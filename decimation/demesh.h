// shapes/trianglemesh.h
#ifndef DECIMATION_DEMESH_H
#define DECIMATION_DEMESH_H

#include "core/common.h"
#include "core/kdtree.h"
#include "decimation/devertex.h"
#include "shapes/trianglemesh.h"
#include "shapes/box.h"

using namespace Raytracer;

namespace Decimation {

class DeMesh : public TriangleMesh {
public:
	DeMesh(string objFile, Material* aMaterial, Matrix44 transform = Matrix44::eye())
		: TriangleMesh(objFile, aMaterial, transform){
	}
	void initialize(bool construction = true){
		//convert to Decimation data, except for pointers in Triangle are Vertex, not DeVertex.
		int n = 0;
		mVexCloud.clear();
		for (Vertex* vex : mVertexs)
			mVexCloud.add(new DeVertex(*vex, n ++));
		Time = 0;
		timeStamp = new int[n];
		mDeTriangles.clear();
		for (Triangle* tri : mTriangles)
			mDeTriangles.push_back(new DeTriangle(*tri, mVexCloud.getData()));
		if (construction)
			mVexCloud.construct();
	}
	string getType(){
		return "DecimationMesh";
	}
	real diagonalLength(){
		return mBoundingBox->getDiagonalLength();
	}
  	int contraction(VertexPair P);
	void decimation(int need, real threshold = 0);
	// void decimation(real ratio, real threshold = 0){
	// 	int m = mTriangles.size();
	// 	decimation(int(m * ratio), threshold);
	// }
	~DeMesh(){
		delete[] timeStamp;
	}
private:
	bool _checkValid(VertexPair P);
	//void _filter(vector<DeTriangle*> &triangles);
	priority_queue<VertexPair> Q;
	KdTree<DeVertex> mVexCloud;
	vector<DeTriangle* > mDeTriangles;
	int* timeStamp;
	int Time;
};

}; // namespace Decimation


#endif // DECIMATION_DEMESH_H
