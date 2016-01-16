// core/film.h
#ifndef CORE_FILM_H
#define CORE_FILM_H

#include "common.h"

namespace Raytracer {

class Film{
public:
	Film(int w, int h):w(w), h(h){
		mName = "";
	}
	void setName(string aName){
		mName = aName;
	}
	int getW(){
		return w;
	}
	int getH(){
		return h;
	}
	virtual void setColor(int x, int y, const Color &color) = 0;
	virtual Color getColor(int x, int y) = 0;
	virtual void read(string path) = 0;
	virtual void show(bool wait = false) = 0;
protected:
	int w, h;
	string mName;
};

}; // namespace Raytracer

#endif // CORE_FILM_H
