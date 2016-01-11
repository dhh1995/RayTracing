// decimation/decimation.h
#ifndef DECIMATION_DECIMATION_H
#define DECIMATION_DECIMATION_H

#include "demesh.h"
#include "detriangle.h"
#include "devertex.h"
#include "core/args.h"

namespace Decimation {

DeMesh* runDecimation(string file, Material* mat, string dumpName, const Args& args);
void Run(const Args& args);

}; // namespace Decimation

#endif // DECIMATION_DECIMATION_H
