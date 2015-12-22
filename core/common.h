// core/common.h
#ifndef CORE_COMMON_H
#define CORE_COMMON_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <assert.h>
#include "geometry.h"

namespace Raytracer {

#define HIT		 1		// Ray hit primitive
#define MISS	 0		// Ray missed primitive
#define INPRIM	-1		// Ray started inside primitive

const int TRACEDEPTH = 0;
const real PI = acos(-1.);
const real INF = 1e18;

typedef Vec3f Color;
const Vec3f ORIGINAL = Vec3f(0., 0., 0.);
const Color WHITE	= Color(1., 1., 1.);
const Color RED		= Color(1., 0., 0.);
const Color GREEN	= Color(0., 1., 0.);
const Color BLUE	= Color(0., 0., 1.);
const Color BLACK	= Color(0., 0., 0.);
const Color BACKGROUND = BLACK;

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::min;

class Scene;
class Camera;
class Primitive;
class Intersection;
class Light;
class Material;

}; // namespace Raytracer

#endif // CORE_COMMON_H