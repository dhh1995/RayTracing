#include "sampler.h"

namespace Raytracer {

void Sampler::concentricSampleDisk(real& dx, real& dy) {
	real r, theta;
	real sx = 2 * getRandReal() - 1;
	real sy = 2 * getRandReal() - 1;

	if (sx == 0.0 && sy == 0.0) {
		dx = 0.0;
		dy = 0.0;
		return;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			r = sx;
			if (sy > 0.0) theta = sy/r;
			else		  theta = 8.0f + sy/r;
		}else {
			r = sy;
			theta = 2.0f - sx/r;
		}
	}else {
		if (sx <= sy) {
			r = -sx;
			theta = 4.0f - sy/r;
		}else {
			r = -sy;
			theta = 6.0f + sx/r;
		}
	}
	theta *= PI / 4.0f;
	dx = r * cos(theta);
	dy = r * sin(theta);
}

}; // namespace Raytracer