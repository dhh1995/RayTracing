// core/args.h
#ifndef CORE_ARGS_H
#define CORE_ARGS_H

#include "common.h"

namespace Raytracer {

struct Args{
	int imageSize;
	int useScene;
	string obj;
	bool debug;
	bool usePhoton;
	bool loadPM;
	bool showHelp;
	int pathIter;

	//photon mapping args
	int photonIter;
	int globalWant;
	int causticWant;
	int searchPhotons;
	real causticSearchRadius;
	real globalSearchRadius;
	real decay;

	//camera args
	real fov;
	real lensRadius;
	real focalDist;

	//Decimation Args
	int useModel;
	int needTriangle;
	real needRatio;
	real threshold;
	string needOption;

	Args(){//default value
		showHelp = false;
		imageSize = 500;
		useScene = 1;
		obj = "cornell_box";
		debug = false;
		usePhoton = false;
		loadPM = false;

		pathIter = 10;

		photonIter = 5;
		globalWant = 10000;
		causticWant = 0;
		searchPhotons = 300;
		causticSearchRadius = 1;
		globalSearchRadius = 0.5;
		decay = 0.7;

		fov = 60;
		lensRadius = 0;
		focalDist = 10;

		useModel = 1;
		needTriangle = 400;
		needRatio = 0.01;
		threshold = 0;
		needOption = "ratio";
	}
	void showHelpInfo(){
		showHelp = true;
		char helpInfo[105];
		sprintf(helpInfo, "--help or -h					Show this help information");
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "--debug 		default = false 	Set debug model on");
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-imgsize [int]		default = %d		the size of image (a, a)", imageSize);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-scene [int]		default = %d		Choose which scene to use", useScene);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-obj [string]		default = %s	Choose which scene to use", obj.c_str());
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "--photon		default = false		set Rendering model as Photon Mapping");
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "--loadpm		default = false		use pre-computed Photon Mapping(according to scene id)");
		colorMessage(helpInfo, 3);

		sprintf(helpInfo, "-ptiter [int] 		default = %d		Iterations for path tracing", pathIter);
		colorMessage(helpInfo, 3);

		colorMessage("Args for Photon Mapping", 5);
		sprintf(helpInfo, "-pmiter [int] 		default = %d		Iterations for photon mapping", photonIter);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-gwant [int] 		default = %d		Number of global photons", globalWant);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-cwant [int] 		default = %d		Number of caustic photons", causticWant);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-nsearch [int] 		default = %d		Number of search photons", searchPhotons);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-gradius [int] 		default = %lf	Search radius for global photon map", globalSearchRadius);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-cradius [int] 		default = %lf	Search Radius for caustic photon map", causticSearchRadius);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-decay [int] 		default = %lf	Decay rate for progressive photon map", decay);
		colorMessage(helpInfo, 3);

		colorMessage("Args for Camera", 5);
		sprintf(helpInfo, "-fov [real] 		default = %lf	Fov for camera", fov);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-lradius [real] 	default = %lf	Radius of lens for camera", lensRadius);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-fdist [real] 		default = %lf	Focal Distance for camera", focalDist);
		colorMessage(helpInfo, 3);

		colorMessage("Args for Decimation", 5);
		sprintf(helpInfo, "-model [int] 		default = %d		Choose which model to use", useModel);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-deoption [string] 	default = %s 	Option model for decimation", needOption.c_str());
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-need [int] 		default = %d 		Set reserve number of triangles", needTriangle);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-ratio [real] 		default = %.3lf		Set reserve ratio of triangles", needRatio);
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-threshold [real]	default = %.3lf		Set threshold for decimation algorithm", threshold);
		colorMessage(helpInfo, 3);
	}
	void parse(int argc, char** argv){
		int ind = 0;
		while (ind < argc){
			string param = argv[ind];
			if (param[0] == '-'){
				char *ptr = argv[++ind];
				if (param == "-h" || param == "--help")
					showHelpInfo(), --ind;
				if (param == "--debug")
					debug = true, --ind;
				else if (param == "-imgsize")
					readBuf(ptr, imageSize);
				else if (param == "-scene")
					readBuf(ptr, useScene);
				else if (param == "-obj")
					obj = argv[ind];
				else if (param == "--photon")
					usePhoton = true, --ind;
				else if (param == "--loadpm")
					loadPM = true, --ind;
				else if (param == "-ptiter")
					readBuf(ptr, pathIter);

				else if (param == "-pmiter")
					readBuf(ptr, photonIter);
				else if (param == "-gwant")
					readBuf(ptr, globalWant);
				else if (param == "-cwant")
					readBuf(ptr, causticWant);
				else if (param == "-nsearch")
					readBuf(ptr, searchPhotons);
				else if (param == "-gradius")
					readBuf(ptr, globalSearchRadius);
				else if (param == "-cradius")
					readBuf(ptr, causticSearchRadius);
				else if (param == "-decay")
					readBuf(ptr, decay);

				else if (param == "-fov")
					readBuf(ptr, fov);
				else if (param == "-lradius")
					readBuf(ptr, lensRadius);
				else if (param == "-fdist")
					readBuf(ptr, focalDist);

				else if (param == "-model")
					readBuf(ptr, useModel);
				else if (param == "-need")
					readBuf(ptr, needTriangle);
				else if (param == "-deoption")
					needOption = argv[ind];
				else if (param == "-ratio")
					readBuf(ptr, needRatio);
				else if (param == "-threshold")
					readBuf(ptr, threshold);
			}
			++ind;
		}
	}
};

}; // namespace Raytracer

#endif // CORE_ARGS_H
