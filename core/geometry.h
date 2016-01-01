// core/geometry.h
#ifndef CORE_GEOMETRY_H
#define CORE_GEOMETRY_H

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <assert.h>
using std::abs;

namespace Raytracer {

typedef double real;
const real EPS = 1e-4;

inline real min(real x, real y, real z){
	if (x < y)
		if (x < z)
			return x;
		else
			return z;
	else
		if (y < z)
			return y;
		else
			return z;
}

inline real max(real x, real y, real z){
	if (x > y)
		if (x > z)
			return x;
		else
			return z;
	else
		if (y > z)
			return y;
		else
			return z;
}


class Vec3f{
public:
	//Constructors
	Vec3f(){
		memset(_p,0,sizeof(real)*_len);
	}
	Vec3f(real x,real y, real z):
		x(x), y(y), z(z){
	}
	Vec3f(const Vec3f& v){
		memcpy(_p,v._p,sizeof(real)*_len);
	}
	//Deconstructor
	virtual ~Vec3f(){}
public:
	//Operators

	//Operator []
	inline real& operator [](int index){
		assert(index>=0&&index<3);
		return _p[index];
	}
	inline const real& operator [](int index) const{
		assert(index>=0&&index<3);
		return _p[index];
	}
	
	//Operator =
	inline Vec3f& operator = (const Vec3f& v){
		memcpy(_p,v._p,sizeof(real)*_len);		
		return (*this);
	}

	//Operators +=, -=, *=, /=
	inline void operator +=(const Vec3f& v){
		for(int i=0;i<_len;i++)
			_p[i] += v._p[i];
	}
	inline void operator +=(real f){
		for(int i=0;i<_len;i++)
			_p[i] += f;
	}
	inline void operator -=(const Vec3f& v){
		for(int i=0;i<_len;i++)
			_p[i] -= v._p[i];
	}
	inline void operator -=(real f){
		for(int i=0;i<_len;i++)
			_p[i] -= f;
	}
	inline void operator *=(const Vec3f& v){
		for(int i=0;i<_len;i++)
			_p[i] *= v._p[i];
	}
	inline void operator *=(real f){
		for(int i=0;i<_len;i++)
			_p[i] *= f;
	}
	inline void operator /=(const Vec3f& v){
		for(int i=0;i<_len;i++)
			_p[i] /= v._p[i];
	}
	inline void operator /=(real f){
		for(int i=0;i<_len;i++)
			_p[i] /= f;
	}

	//Operators +, -, *, /
	// inline Vec3f operator +(const Vec3f&v) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] + v[i];
	// 	return res;
	// }
	// inline Vec3f operator +(real f) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] + f;
	// 	return res;
	// }
	// inline Vec3f operator -(const Vec3f&v) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] - v[i];
	// 	return res;
	// }
	// inline Vec3f operator -(real f) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] - f;
	// 	return res;
	// }
	// inline Vec3f operator *(const Vec3f&v) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] * v[i];
	// 	return res;
	// }
	// inline Vec3f operator *(real f) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] * f;
	// 	return res;
	// }
	// inline Vec3f operator /(const Vec3f&v) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] / v[i];
	// 	return res;
	// }
	// inline Vec3f operator /(real f) const{
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = (*this)[i] / f;
	// 	return res;
	// }
	// inline Vec3f operator - () const {
	// 	Vec3f res;
	// 	for(int i=0;i<_len;i++)
	// 		res[i] = -(*this)[i];
	// 	return res;
	// }
	inline Vec3f operator - () const {
		return Vec3f(-x, -y, -z);
	}
	inline bool operator <(const Vec3f &v) const{
		return x < v.x && y < v.y && z < v.z;
	}
	inline bool operator !=(const Vec3f &v) const{
		return abs(x-v.x)>EPS || abs(y-v.y)>EPS || abs(z-v.z)>EPS ;
	}
	inline bool operator >(const Vec3f &v) const{
		return x > v.x && y > v.y && z > v.z;
	}

	friend inline Vec3f operator + ( const Vec3f& v1, const Vec3f& v2 ) { return Vec3f( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend inline Vec3f operator - ( const Vec3f& v1, const Vec3f& v2 ) { return Vec3f( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }

	friend inline Vec3f operator / ( const Vec3f& v, real f ) { return Vec3f( v.x / f, v.y / f, v.z / f ); }
	friend inline Vec3f operator * ( const Vec3f& v, real f ) { return Vec3f( v.x * f, v.y * f, v.z * f ); }
	friend inline Vec3f operator * ( const Vec3f& v1, const Vec3f& v2 ) { return Vec3f( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend inline Vec3f operator * ( real f, const Vec3f& v ) { return Vec3f( v.x * f, v.y * f, v.z * f ); }

public:
	inline real L2(){
		return x * x + y * y + z * z;
	}
	inline real length(){
		return sqrt(L2());
	}
	inline Vec3f Normalize(){
		real len = length();
		//if(len>1e-6)
		(*this) *= 1.0f / len;
		return *this;
	}
	// void read(FILE* fp){
	// 	fscanf(fp, "%lf %lf %lf", &x, &y, &z);
	// }
	inline real getMin(){
		return min(x, y, z);
	}
	inline real getMax(){
		return max(x, y, z);
	}
	inline int argMax() const{
		if (x > y)
			if (x > z)
				return 0;
			else
				return 2;
		else
			if (y > z)
				return 1;
			else
				return 2;
	}
	void read(){
		scanf("%f %f %f", &x, &y, &z);
	}
	void prt() const{
	    printf("%f %f %f\n",x,y,z);    
	}
	void print() const{
	    printf("%f %f %f\n",x,y,z);    
	}

public:
	union{
		struct{
			real _p[3];
		};
		struct{
			real x,y,z;
		};
		struct{
			real r,g,b;
		};
	};
	enum {_len = 3};
};


inline real dot(const Vec3f &A, const Vec3f &B){
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

inline Vec3f cross(const Vec3f &A, const Vec3f &B){
	return Vec3f(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

inline real det(const Vec3f &A, const Vec3f &B, const Vec3f &C){
	return A[0] * B[1] * C[2] + A[1] * B[2] * C[0] + A[2] * B[0] * C[1]
		 - A[2] * B[1] * C[0] - A[1] * B[0] * C[2] + A[0] * B[2] * C[1];
}

inline void minimize(Vec3f &A, const Vec3f &B){
	for (int i=0;i<3;++i)
		if (B[i] < A[i])
			A[i] = B[i];
}

inline void maximize(Vec3f &A, const Vec3f &B){
	for (int i=0;i<3;++i)
		if (B[i] > A[i])
			A[i] = B[i];
}


class Ray{
public:
	Ray();
	//Ray(const Vec3f &o, const Vec3f &d, real start = 0.0, real end = INF, real time = 0.0, int depth = 0);
	Ray(const Vec3f &o, const Vec3f &d, int aFilmX = -1, int aFilmY = -1)
		: o(o), d(d), mFilmX(aFilmX), mFilmY(aFilmY){
		this->d.Normalize();
	}
	void setDir(Vec3f dir){
		d = dir;
	}
	inline Vec3f operator()(real t) const {
		return o + d * t;
	}
	void prt() const{
		printf("o: ");
		o.print();
		printf("d: ");
		d.print();
		printf("filmX %d filmY %d\n",mFilmX, mFilmY);
	}
	Vec3f o;
	Vec3f d;
	int mFilmX, mFilmY;
	//real mint, maxt;
	//real time;
	//int depth;
	//real energy;
};

struct Matrix44{
	Matrix44();
	Matrix44(real mat[4][4]);
    Matrix44(real t00, real t01, real t02, real t03,
              real t10, real t11, real t12, real t13,
              real t20, real t21, real t22, real t23,
              real t30, real t31, real t32, real t33);
    /*real* operator[](int id){
    	assert(id>=0 && id<=3);
    	return m[id];
    }*/
	Matrix44 Transpose();
	real m[4][4];
};


}; // namespace Raytracer

#endif // CORE_GEOMETRY_H