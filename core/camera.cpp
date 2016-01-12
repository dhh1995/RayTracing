#include "camera.h"

namespace Raytracer {

void ProjectiveCamera::convert(int x, int y, real& dx, real& dy){
	int w = getFilm()->getW(), h = getFilm()->getH();
	dx = l + (r-l) / (w-1) * (w-1-x);
	dy = l + (r-l) / (h-1) * (h-1-y);
}

Ray ProjectiveCamera::sampleRay(int x, int y){
	real dx, dy;
	convert(x, y, dx, dy);
	return Ray(mPos, mLookAt + mI * dx + mJ * dy, x, y);
}

Ray PerspectiveCamera::sampleRay(int x, int y){
	real dx, dy;
	convert(x, y, dx, dy);
	Vec3f dir = mLookAt + mI * dx + mJ * dy;
	Ray ray = Ray(mPos, dir, x, y);
	if (mLensRadius > 1e-8){
		real u, v;
		Sampler::concentricSampleDisk(u, v);
		u *= mLensRadius, v *= mLensRadius;
		Vec3f Pfocus = mPos + dir * mFocalDist;
		ray.o = mPos + mI * u + mJ * v;
		ray.d = (Pfocus - ray.o).Normalize();
	}
	return ray;
}

}; // namespace Raytracer