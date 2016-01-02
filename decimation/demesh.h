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
		int n = 0;
		for (Vertex* vex : mVertexs){
			DeVertex* meshVex = new DeVertex(*vex, n ++);
			mVexCloud.add(meshVex);
		}
		merged = new bool[n];
		for (Triangle* tri : mTriangles){
			DeTriangle* deTri= new DeTriangle(tri);
			mDeTriangles.push_back(deTri);
		}
	}
	string getType(){
		return "DecimationMesh";
	}
  	void contraction(VertexPair P);
	void decimation(real percent, real threshold = 0);
	~DeMesh(){
		delete[] merged;
	}
private:
	bool _checkValid(VertexPair P);
	priority_queue<VertexPair> Q;
	KdTree<DeVertex> mVexCloud;
	vector<DeTriangle* > mDeTriangles;
	bool* merged;
};

}; // namespace Decimation


#endif // DECIMATION_DEMESH_H
