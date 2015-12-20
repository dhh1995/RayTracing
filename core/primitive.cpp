#include "primitive.h"

namespace Raytracer {

void Primitive::setName(char* aName)
{
	delete mName;
	mName = new char[strlen(aName) + 1]; 
	strcpy(mName, aName); 
}

}; // namespace Raytracer
