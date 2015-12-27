// core/kdtree.h
#ifndef CORE_KDTREE_H
#define CORE_KDTREE_H

#include "common.h"
#include "photon.h"
#include "shapes/trianglemesh.h"
#include "shapes/box.h"

namespace Raytracer {

//balanced KdTree for Photon

bool PhotonCmp(const Photon* A, const Photon* B);

//template <class T>
class KdTree{
public:
	struct KdNode{
		Photon* t;
		Box* b;
		real split;
		short ch;
		short dim;
	};
	KdTree(){
		clear();
	}
	void clear(){
		n = 0;
		mData.clear();
	}
	void add(Photon* x){
		++ n;
		mData.push_back(x);
	}
	void build(int root, int l, int r);
	void construct(){
		int N = n * 2;
		a = new KdNode[N];
		build(1, 0, n);
	}
	pair<real, Photon* > getKthPhoton(int k){
		return res[k];
	}
	int getKNearest(const Vec3f& pos, int K);
public:
	static int mDim;
private:
	void findKNearest(int root);
	void addToHeap(Photon* t);
	int mLimit;
	real mDist;
	int n, m;
	KdNode* a;
	vector<Photon* > mData;
	pair<real, Photon* > res[MAX_K_NEAREST];
	Vec3f aPos;
};

//unbalanced KdTree using SAH

class KdTreeTri{
public:
	struct KdNode{
		short isLeaf, dim;
		real split;
		Triangle* x;
		KdNode* left;
		KdNode* right;
	};
	KdTreeTri(){
		clear();
	}
	void clear(){
		n = 0;
		a.clear();
	}
	void add(Triangle* x){
		a.push_back(x);
	}
	void construct();
private:
	int n;
	vector<Triangle* > a;
};

}; // namespace Raytrace

#endif // CORE_KDTREE_H
