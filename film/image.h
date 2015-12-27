// film/image.h
#ifndef FILM_IMAGE_H
#define FILM_IMAGE_H

#include "core/common.h"
#include "core/film.h"
#ifndef LOCAL
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/core/core.hpp>
	#include <opencv2/opencv.hpp>
#else
	#include <opencv2/highgui.hpp>
	#include <opencv2/core.hpp>
	#include <opencv2/opencv.hpp>
#endif
using cv::Mat;
using cv::Vec3b;
using cv::imwrite;
using cv::imread;
using cv::imshow;
using cv::waitKey;

namespace Raytracer {

class Image : public Film {
public:
	Image(string path):Film(-1, -1){
		read(path);
	}
	Image(int w, int h):Film(w, h){
		mImg = Mat(w, h, CV_8UC3);
	}
	static Vec3b toVec3b(const Color &color);
	static Color toColor(const Vec3b &c);
	void setColor(int x, int y, const Color &color);
	Color getColor(int x, int y);
	void read(string path);
	void show();
	Mat mImg;
};

}; // namespace Raytrace

#endif // FILM_IMAGE_H
