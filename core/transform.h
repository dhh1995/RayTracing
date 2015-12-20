// core/transform.h
#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "common.h"

namespace Raytracer {

struct Matrix44{
	Matrix44();
	Matrix44(real mat[4][4]);
    Matrix44(real t00, real t01, real t02, real t03,
              real t10, real t11, real t12, real t13,
              real t20, real t21, real t22, real t23,
              real t30, real t31, real t32, real t33);
    /*real* operator[](int id){
    	assert(id>=0 && id<=3);
    	return m[id];
    }*/
	Matrix44 Transpose();
	real m[4][4];
};

class Transform{
public:
	Transform();
	Transform(real mat[4][4]);
	Transform(const Matrix44 &mat);
private:
	Matrix44 m;
};

}; // namespace Raytrace

#endif // CORE_TRANSFORM_H
