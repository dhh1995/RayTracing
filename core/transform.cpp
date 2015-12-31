#include "transform.h"

namespace Raytracer {

Transform::Transform(){
}

Transform::Transform(real mat[4][4]){
	m = Matrix44(mat);
}

Transform::Transform(const Matrix44 &mat)
	:m(mat){
		
}

}; // namespace Raytrace
