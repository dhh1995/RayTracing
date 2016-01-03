// core/geometry.h
#ifndef CORE_GEOMETRY_H
#define CORE_GEOMETRY_H

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>
#include <assert.h>
using std::abs;
using std::vector;
using std::swap;
using std::pair;
using std::make_pair;

namespace Raytracer {

typedef double real;
const real EPS = 1e-4;

inline bool equal(real x, real y){
	return abs(x - y) < EPS;
}

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
	Matrix44(real eye = 0.0f){
		m[0][0]	=	m[1][1] =	m[2][2] =	m[3][3] = eye;
					m[0][1] =	m[0][2] =	m[0][3] =
		m[1][0]				=	m[1][2] =	m[1][3] = 
		m[2][0] =	m[2][1]				=	m[2][3] =
		m[3][0] =	m[3][1] =	m[3][2]				= 0.0;
	}

	Matrix44(const real mat[4][4]){
		memcpy(m, mat, 16*sizeof(real));
	}
	Matrix44(const vector<real> &v){	//from vector 4 real m = v v^T
		assert(v.size() == 4);
		for (int i = 0; i < 4; ++ i){
			m[i][i] = v[i] * v[i];
			for (int j = 0; j < i; ++ j)
				m[i][j] = m[j][i] = v[i] * v[j];
		}
	}
	Matrix44(real t00, real t01, real t02, real t03,
			 real t10, real t11, real t12, real t13,
			 real t20, real t21, real t22, real t23,
			 real t30, real t31, real t32, real t33){
		m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
		m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
		m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
		m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
	}

	/*real* operator[](int id){
		assert(id>=0 && id<=3);
		return m[id];
	}*/
	Matrix44 operator += (const Matrix44& A) {
		for (int i = 0; i < 4; ++ i)
			for (int j = 0; j < 4; ++ j)
				m[i][j] += A.m[i][j];
		return *this;
	}
	Matrix44 operator + (const Matrix44& A) const{
		Matrix44 res;
		for (int i = 0; i < 4; ++ i)
			for (int j = 0; j < 4; ++ j)
				res.m[i][j] = m[i][j] + A.m[i][j];
		return res;
	}

	Matrix44 operator * ( const Matrix44& A) const{
		Matrix44 res;
		for (int k = 0; k < 4; ++ k)
			for (int i = 0; i < 4; ++ i)
				for (int j = 0; j < 4; ++ j)
					res.m[i][j] += m[i][k] * A.m[k][j];
		return res;
	}

	pair<Vec3f, real> operator *(const Vec3f& A) const{	//assumme last row is 0,0,0,1
		Vec3f res;
		for (int i = 0; i < 3; ++ i){
			res[i] = m[i][3];
			for (int j = 0; j < 3; ++ j)
				res[i] += m[i][j] * A[j];
		}
		real res3 = m[3][0] * A[0] + m[3][1] * A[1] + m[3][2] * A[2] + m[3][3] /*  *1  */ ;
		return make_pair(res, res3);
	}

	Matrix44 transpose() const{
	   return Matrix44(	m[0][0], m[1][0], m[2][0], m[3][0],
						m[0][1], m[1][1], m[2][1], m[3][1],
						m[0][2], m[1][2], m[2][2], m[3][2],
						m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	Matrix44 inverse() const{
		Matrix44 org(m), inv(1.0f);
		for (int i = 0; i < 4; ++ i){
			for (int j = i+1; equal(org.m[i][i], 0) && j < 4; ++ j)
				if (!equal(org.m[i][j], 0)){
					for (int k = i; k < 4; ++ k)
						swap(org.m[i][k], org.m[j][k]);
					for (int k = 0; k < 4; ++ k)
						swap(inv.m[i][k], inv.m[j][k]);
				}
			if (equal)
			for (int k = 0; k < 3; ++ k)
				inv.m[i][k] /= org.m[i][i];
			for (int k = 3; k >= i; --k)
				org.m[i][k] /= org.m[i][i];
			for (int j = 0; j < 4; ++ j) if (j != i)
				if (!equal(org.m[j][i], 0)){
					for (int k = 0; k < 4; ++ k)
						inv.m[j][k] -= org.m[j][i] * inv.m[i][k];
					for (int k = 3; k >= i; -- k)
						org.m[j][k] -= org.m[j][i] * org.m[i][k];
				}
		}
		return inv;
	}

	static Matrix44 translation(const Vec3f& A){
		return Matrix44( 1 ,  0 ,  0 , A.x, 
						 0 ,  1 ,  0 , A.y, 
						 0 ,  0 ,  1 , A.z,
						 0 ,  0 ,  0 ,  1 );
	}

	static Matrix44 scale(const Vec3f& A){
		return Matrix44(A.x,  0 ,  0 ,  0 , 
						 0 , A.y,  0 ,  0 , 
						 0 ,  0 , A.z,  0 , 
						 0 ,  0 ,  0 ,  1 );
	}

	static Matrix44 scale(real s){
		return scale(Vec3f(s, s, s));
	}

	static Matrix44 rotateX(real theta){
		return Matrix44( 0 ,	0		, 		1	  , 0, 
						 0 , cos(theta)	, -sin(theta) , 0, 
						 0 , sin(theta)	,  cos(theta) , 0, 
						 0 ,	0		, 		0	  , 1);
	}

	static Matrix44 rotateY(real theta){
		return Matrix44(  cos(theta) , 0 , sin(theta) , 0, 
								0	 , 0 , 		0	  , 0, 
						 -sin(theta) , 0 , cos(theta) , 0, 
								0	 , 0 ,		0	  , 1);
	}

	static Matrix44 rotateZ(real theta){
		return Matrix44( cos(theta)	, -sin(theta) , 0 , 0,
						 sin(theta)	,  cos(theta) , 0 , 0,
							0		,		0	  , 1 , 0, 
							0		,		0	  , 0 , 1);
	}

	void prt() const{
		for (int i = 0; i < 4; ++ i){
			for (int j = 0; j < 4; ++ j)
				printf("%.6lf ", m[i][j]);
			puts("");
		}
	}

	real m[4][4];
};


}; // namespace Raytracer

#endif // CORE_GEOMETRY_H