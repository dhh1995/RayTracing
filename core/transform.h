// core/transform.h
#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "common.h"

namespace Raytracer {


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
