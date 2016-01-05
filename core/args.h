// core/args.h
#ifndef CORE_ARGS_H
#define CORE_ARGS_H

#include "common.h"

namespace Raytracer {

struct Args{
	int useScene;
	int useModel;
	int needTriangle;
	real needRatio;
	real threshold;
	string needOption;
	bool debug;
	bool showHelp;
	Args(){
		//default
		useScene = 666;
		useModel = 1;
		needTriangle = 400;
		needRatio = 0.01;
		threshold = 0;
		needOption = "ratio";
		debug = false;
		showHelp = false;
	}
	void showHelpInfo(){
		showHelp = true;
		char helpInfo[105];
		sprintf(helpInfo, "--help or -h					Show this help information");
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "--debug 		default = false 	Set debug model on");
		colorMessage(helpInfo, 3);
		sprintf(helpInfo, "-scene [int]		default = %d		Choose which scene to use", useScene);
		colorMessage(helpInfo, 3);
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
				else if (param == "-scene")
					readBuf(ptr, useScene);
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
