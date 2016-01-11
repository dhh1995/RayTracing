#include "devertex.h"

namespace Decimation {

real DeVertex::computeCost(DeVertex* A, DeVertex* B, Vec3f& v){
	Matrix44 Q = A->getQuadMatrix() + B->getQuadMatrix();
	v = (Q.inverse() * ORIGINAL).first;
	// v^T Q v 
	//  x,y,z,1 * a00,a01,a02,a03, * x
	//			  a10,a11,a12,a13,   y
	//			  a20,a21,a22,a23,   z
	//			  a30,a31,a32,a33,   1
	pair<Vec3f, real> res = Q * v;
	return dot(v, res.first) + res.second;
}

}; // namespace Decimation
