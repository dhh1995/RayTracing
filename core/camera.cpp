#include "camera.h"

namespace Raytracer {

Ray ProjectiveCamera::generateRay(real dx, real dy, int i, int j){
	//mLookAt.print();
	return Ray(mPos, mLookAt + mI * dx + mJ * dy, i, j);
}

vector<Ray> ProjectiveCamera::generateRays(){
	vector<Ray> rays;
	int w = getFilm()->getW(), h = getFilm()->getH();
	//real di = mFov / 180 * PI / (w-1), dj = mFov /  180 * PI / (h-1);
	real l = tan(- mArc / 2), r = tan(mArc / 2);
	printf("camera left = %lf right = %lf\n",l,r);
	for (int i=0;i<w;++i)
		for (int j=0;j<h;++j){
			//if (i != w/2 || j != h/2) continue;
			//if ((i%30 == 0 || i+1 == w) && (j%40==0 || j+1 ==h))
			//	printf("%lf %lf\n",tan(mArc / (w-1) * i - mArc / 2), tan(mArc / (h-1) * j - mArc / 2));
			//rays.push_back(generateRay(tan(mArc / (w-1) * i - mArc / 2), tan(mArc / (h-1) * j - mArc / 2), i, j));
			rays.push_back(generateRay( l + (r-l)/(w-1) * (w-1-i), l + (r-l) / (h-1) * (h-1-j) , i, j));
		}
	return rays;
}

}; // namespace Raytracer