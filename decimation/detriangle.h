// decimation/detriangle.h
#ifndef DECIMATION_DETRIANGLE_H
#define DECIMATION_DETRIANGLE_H

#include "core/common.h"
#include "shapes/triangle.h"

using namespace Raytracer;

namespace Decimation {

// only IDs(a, b, c) are correct while decimation.
class DeTriangle : public Triangle{
public:
	DeTriangle(Triangle* tri): Triangle(*tri){
		degeneration = false;
	}
	string getType(){
		return "DeTriangle";
	}
	vector<real> getPlaneParam(){
		return vector<real>({mNorm.x, mNorm.y, mNorm.z, mD});
	}
	vector<int> getVexIDs(){
		return vector<int>({a,b,c});
	}
	int* findVertexID(int id){
		if (a == id)
			return &a;
		if (b == id)
			return &b;
		if (c == id)
			return &c;
		return NULL;
	}
	int getVexID(int i){
		return i == 0 ? a : i == 1 ? b : c;
	}
	void setDegeneration(bool value = true){
		degeneration = value;
	}
	bool isDegeneration(){
		return degeneration;
	}
	void prt(){
		colorMessage("Triangle:", 3);
		printf("%d %d %d\n", a, b, c);
	}
private:
	bool degeneration;
};

}; // namespace Decimation


#endif // DECIMATION_DETRIANGLE_H
