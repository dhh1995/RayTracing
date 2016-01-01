// core/common.h
#ifndef CORE_COMMON_H
#define CORE_COMMON_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>
#include <assert.h>
#include "geometry.h"

namespace Raytracer {

const bool HIT = true;
const bool MISS = false;
//#define HIT		true		// Ray hit primitive
//#define MISS		false		// Ray missed primitive
//#define INPRIM	-1			// Ray started inside primitive

const int TRACEDEPTH = 5;
const int MAX_K_NEAREST = 112345;
//const int MAX_KDTREE_LEAF_SIZE = 7;
const real PI = acos(-1.);
const real INF = 1e18;

typedef Vec3f Color;
const Vec3f ZERO 	 = Vec3f(0., 0., 0.);
const Vec3f ORIGINAL = Vec3f(0., 0., 0.);

const Color WHITE	= Color(1., 1., 1.);
const Color RED		= Color(1., 0., 0.);
const Color GREEN	= Color(0., 1., 0.);
const Color BLUE	= Color(0., 0., 1.);
const Color BLACK	= Color(0., 0., 0.);
const Color LIGHTPINK	= Color(255, 182, 193) / 255.;
const Color PINK 		= Color(255, 192, 203) / 255.;
const Color VIOLET		= Color(238, 130, 238) / 255.;
const Color PURPLE		= Color(128,  0 , 128) / 255.;
const Color DARKBLUE	= Color( 0 ,  0 , 139) / 255.;
const Color SKYBLUE 	= Color(135, 206, 235) / 255.;
const Color CYAN 		= Color( 0 , 255, 255) / 255.;
const Color LIGHTGREEN	= Color(144, 238, 144) / 255.;
const Color DARKGREEN	= Color( 0 , 100,  0 ) / 255.;
const Color YELLOW 		= Color(255, 255,  0 ) / 255.;

const Color BACKGROUND = BLACK;

using std::priority_queue;
using std::string;
using std::pair;
using std::make_pair;
using std::swap;
using std::cout;
using std::endl;
using std::min;

inline bool same(real x, real y){
	if (x < y + EPS && y < x + EPS)
		return true;
	return false;
}

inline void colorMessage(string message, int color){
	char* cmd = new char[message.size() + 50];
	sprintf(cmd, "echo '\e[1;%dm%s\e[0m'", color + 30, message.c_str());
	system(cmd);
	delete[] cmd;
}

inline void progressMessage(string message){
	colorMessage(message, 2); //red
}

template<class T> class KdTree;
class Photon;
typedef KdTree<Photon> PhotonMap;
class MeshVertex;
typedef KdTree<MeshVertex> VertexCloud;

//class Triangle;
class Scene;
class Camera;
class Primitive;
class Intersection;
class Light;
class Material;

}; // namespace Raytracer

#endif // CORE_COMMON_H