// shapes/trianglemesh.h
#ifndef SHAPES_TRIANGLEMESH_H
#define SHAPES_TRIANGLEMESH_H

#include "core/common.h"
#include "core/kdtree.h"
#include "decimation/devertex.h"
#include "shapes/trianglemesh.h"

using namespace RayTracer;

namespace Decimation {

class DeMesh : public TriangleMesh{
public:
	DeMesh(string objFile, Material* aMaterial, Vec3f trans = ORIGINAL, real scale = 1.0f)
		: TriangleMesh(objFile, aMaterial, trans, scale){
		}
	string getType(){
		return "DecimationMesh";
	}
  	void contraction(VertexPair P);
	void decimation(real percent, real threshold = 0);
private:
	priority_queue<VertexPair> Q;
	VertexCloud mVexCloud;
};

}; // namespace Decimation


#endif // SHAPES_TRIANGLEMESH_H
