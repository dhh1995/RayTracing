// decimation/detriangle.h
#ifndef DECIMATION_DETRIANGLE_H
#define DECIMATION_DETRIANGLE_H

#include "core/common.h"
#include "shapes/triangle.h"

using namespace Raytracer;

namespace Decimation {

class DeTriangle : public Triangle{
public:
	DeTriangle(Triangle* tri): Triangle(*tri){
	}
	string getType(){
		return "DeTriangle";
	}
	vector<real> getPlaneParam(){
		return vector<real>({mNorm.x, mNorm.y, mNorm.z, mD});
	}
	int getVexID(int i){
		return i == 0 ? a : i == 1 ? b : c;
	}
	void prt(){
		colorMessage("Triangle:", 3);
		A->prt();
		B->prt();
		C->prt();
	}
private:
};

}; // namespace Decimation


#endif // DECIMATION_DETRIANGLE_H
