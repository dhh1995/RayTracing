// decimation/detriangle.h
#ifndef DECIMATION_DETRIANGLE_H
#define DECIMATION_DETRIANGLE_H

#include "core/common.h"
#include "shapes/triangle.h"

using namespace Raytracer;

namespace Decimation {

// only IDs(a, b, c) are correct while decimation.
class DeVertex;
class DeTriangle : public Triangle{
public:
	DeTriangle(const Triangle& tri, vector<DeVertex*>& vertexes): Triangle(tri){
		A = (Vertex*)vertexes[a];
		B = (Vertex*)vertexes[b];
		C = (Vertex*)vertexes[c];
		degeneration = false;
	}
	string getType(){
		return "DeTriangle";
	}
	vector<real> getPlaneParam(){
		return vector<real>({mNorm.x, mNorm.y, mNorm.z, mD});
	}
	void setVexIDs(int* id){
		a = id[0];
		b = id[1];
		c = id[2];
	}
	bool haveVertex(Vertex* vex){
		if (A == vex) return true;
		if (B == vex) return true;
		if (C == vex) return true;
		return false;
	}
	bool changeVertex(Vertex *from, Vertex* to){
		if (A == from)
			return A = to, true;
		if (B == from)
			return B = to, true;
		if (C == from)
			return C = to, true;
		return false;
	}
	// int* findVertexId(int id){
	// 	if (a == id)
	// 		return &a;
	// 	if (b == id)
	// 		return &b;
	// 	if (c == id)
	// 		return &c;
	// 	return NULL;
	// }
	Vertex* getVex(int i){
		return i == 0 ? A : i == 1 ? B : C;
	}
	void setDegeneration(bool value = true){
		degeneration = value;
	}
	bool isDegeneration(){
		return degeneration;
	}
private:
	bool degeneration;
};

}; // namespace Decimation


#endif // DECIMATION_DETRIANGLE_H
