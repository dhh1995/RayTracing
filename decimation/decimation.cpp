#include "demesh.h"
#include "detriangle.h"
#include "devertex.h"
#include "core/args.h"

namespace Decimation {

DeMesh* runDecimation(string file, Material* mat, string dumpName, const Args& args){
	char name[55];
	DeMesh* obj = new DeMesh(file, mat);
	int m = obj->getTriangles().size();
	int need = args.needTriangle;
	if (args.needOption == "ratio")
		need = int(m * args.needRatio);
	real threshold = args.threshold;
	if (threshold < 0)
		threshold = obj->diagonalLength() / (-threshold);
	progressMessage("start decimation");
	obj->decimation(need, threshold);

	sprintf(name, "%s_res_%d.obj", dumpName.c_str(), need);
	obj->dump(name);
	progressMessage("end dumping");

	return obj;
}

void Run(const Args& args){
	enum OBJ{
		ARMA,
		BLOCK,
		BUDDHA,
		BUNNY,
		CUBE,
		DINASAUR,
		DRAGON,
		FANDISK,
		HORSE,
		KITTEN,
		ROCKER_ARM,
		SPHERE,
	};
	int testModel = args.useModel;
	DeMesh *resObj;

	progressMessage("start loading");
	switch(testModel){
	case ARMA:
		resObj = runDecimation("test_data/Arma.obj", NULL, "arma", args);
		break;
	case BLOCK:
		resObj = runDecimation("test_data/block.obj", NULL, "block", args);
		break;
	case BUDDHA:
		resObj = runDecimation("test_data/Buddha.obj", NULL, "buddha", args);
		break;
	case BUNNY:
		resObj = runDecimation("test_data/bunny.fine.obj", NULL, "bunny", args);
		break;
	case CUBE:
		resObj = runDecimation("test_data/cube.obj", NULL, "cube", args);
		break;
	case DINASAUR:
		resObj = runDecimation("test_data/dinosaur.2k.obj", NULL, "dinosaur", args);
		break;
	case DRAGON:
		resObj = runDecimation("test_data/fixed.perfect.dragon.100K.0.07.obj", NULL, "dragon", args);
		break;
	case FANDISK:
		resObj = runDecimation("test_data/fandisk.18k.obj", NULL, "fandisk", args);
		break;
	case HORSE:
		resObj = runDecimation("test_data/horse.fine.90k.obj", NULL, "horse", args);
		break;
	case KITTEN:
		resObj = runDecimation("test_data/kitten.50k.obj", NULL, "kitten", args);
		break;
	case ROCKER_ARM:
		resObj = runDecimation("test_data/rocker-arm.18k.obj", NULL, "rocker-arm", args);
		break;
	case SPHERE:
		resObj = runDecimation("test_data/sphere.obj", NULL, "sphere", args);
		break;
	}
}

}; // namespace Decimation
