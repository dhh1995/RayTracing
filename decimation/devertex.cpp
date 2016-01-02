#include "devertex.h"

namespace Decimation {

real DeVertex::computeCost(DeVertex* A, DeVertex* B, Vec3f& v){
	Matrix44 Q = A->getQuadMatrix() + B->getQuadMatrix();
	v = Q.inverse() * ORIGINAL;
	return dot(v, Q * v) + Q.m[3][3];
}

}; // namespace Decimation
