#include "geometry.h"

namespace Raytracer {

//----------------------------------Ray-----------------------------------
// Ray::Ray():
// 	mint(0.0), maxt(INF), time(0.0), depth(0){
// }

// Ray::Ray(const Vec3f &o, const Vec3f &d, real mint, real maxt, real time, int depth):
// 	o(o), d(d), mint(mint), maxt(maxt), time(time), depth(depth){
// }


Matrix44::Matrix44(){
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
		      m[0][1] = m[0][2] = m[0][3] =
	m[1][0]           = m[1][2] = m[1][3] = 
	m[2][0] = m[2][1]           = m[2][3] =
	m[3][0] = m[3][1] = m[3][2]           = 0.0;
}

Matrix44::Matrix44(real mat[4][4]){
    memcpy(m, mat, 16*sizeof(real));
}

Matrix44::Matrix44(real t00, real t01, real t02, real t03,
                   real t10, real t11, real t12, real t13,
                   real t20, real t21, real t22, real t23,
                   real t30, real t31, real t32, real t33) {
    m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
    m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
    m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
    m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
}

Matrix44 Matrix44::Transpose(){
   return Matrix44(m[0][0], m[1][0], m[2][0], m[3][0],
                    m[0][1], m[1][1], m[2][1], m[3][1],
                    m[0][2], m[1][2], m[2][2], m[3][2],
                    m[0][3], m[1][3], m[2][3], m[3][3]);
}


}; // namespace Raytracer