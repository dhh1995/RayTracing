#include "demesh.h"

namespace Decimation {

void TriangleMesh::contraction(VertexPair P){

}

void TriangleMesh::decimation(real percent, real threshold){
	int m = mTriangles.size(), need = int(m * percent);
	int n = 0;
	for (Vertex* vex : mVertexs){
		MeshVertex* meshVex = new MeshVertex(*vex, n ++);
		mVexCloud.add(meshVex);
	}
	vector<MeshVertex*> meshVertexs = mVexCloud.getData();
	mVexCloud.construct();
	for (Triangle* tri : mTriangles){
		Matrix44 quadMatrix(tri->getPlaneParam());
		for (int i = 0; i < 3; ++ i)
			meshVertexs[tri->getVexID(i)]->merge(quadMatrix);
	}
	for (MeshVertex* vex : meshVertexs){
		vector<MeshVertex* > res;
		mVexCloud.findInBall(res, mVexCloud.root, vex->getPos(), threshold * threshold);
		for (MeshVertex* near : res){

		}
	}
	while (!Q.empty() && n > need){
		VertexPair pair = Q.top();
		Q.pop();

	}
}

}; // namespace Decimation
