#include "image.h"

namespace Raytracer {

Vec3b Image::toVec3b(const Color &c){
	return Vec3b(min(255, int(c.b * 255)),  min(255, int(c.g * 255)), min(255, int(c.r * 255)));
}

Color Image::toColor(const Vec3b &c){
	return Color(c[2] / 255. , c[1] / 255. , c[0] / 255. );
}

void Image::setColor(int x, int y, const Color &color){
	mImg.at<Vec3b>(x,y) = toVec3b(color);
}

Color Image::getColor(int x, int y){
	//printf("%d %d %d %d\n",x,y,h,w);
	return toColor(mImg.at<Vec3b>(x,y));
}

void Image::read(string path){
	mImg = imread(path.c_str());
	w = mImg.rows;
	h = mImg.cols;
}

void Image::show(bool wait){
	string imgName = mName;
	imgName += ".jpg";
	imwrite(imgName, mImg);
#ifdef LOCAL
	imshow(mName, mImg);
	if (wait)
		waitKey(0);
#endif
}

}; // namespace Raytracer
