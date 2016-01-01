// core/kdtree.h
#ifndef CORE_KDTREE_H
#define CORE_KDTREE_H

#include "common.h"
#include "photon.h"
#include "vertex.h"
#include "shapes/triangle.h"
#include "shapes/box.h"

namespace Raytracer {

//balanced KdTree for Photon

//TO be validate {

template<class T>
class KdTree{
public:
	struct KdNode{
		T* t;
		Box* b;
		real split;
		short ch;
		short dim;
		~KdNode(){
			delete b;
		}
	};

	struct KdCmp {
		KdCmp(short dim = 0) : mDim(dim) {
		}
		bool operator ()(const T *A, const T *B) {
			return A->getAxis(mDim) < B->getAxis(mDim);
		}
		short mDim;
	};
	KdTree(int MaxK = MAX_K_NEAREST){
		a = NULL;
		res = new pair<real, T* >[MaxK];
		clear();
	}
	void clear(){
		n = 0;
		mData.clear();
	}
	void add(T* x){
		++ n;
		mData.push_back(x);
	}
	void build(int root, int l, int r){
		KdNode* cur = &a[root];
		cur->b = new Box;
		for (int i = l; i < r; ++ i)
			cur->b->update(mData[i]->getPos());
		cur->dim = mDim = cur->b->argMaxDiff();
		KdCmp cmp = KdCmp(mDim);
		int m = (l + r) / 2;
		nth_element(mData.begin() + l, mData.begin() + m, mData.begin() + r, cmp);
		cur->t = mData[m];
		cur->split = cur->t->getAxis(cur->dim);
		cur->ch = 0;
		if (l < m)
			build(root * 2, l, m), cur->ch |= 1;
		if (m + 1 < r)
			build(root * 2 + 1, m + 1, r), cur->ch |= 2;
	}
	void construct(){
		int N = n * 2;

		if (a != NULL)
			delete[] a;
		a = new KdNode[N];
		root = 1;
		build(1, 0, n);
	}
	pair<real, T* > getKthT(int k){
		return res[k];
	}
	int getKNearest(const Vec3f& pos, int K){
		m = 0, mLimit = K;
		aPos = pos;
		_findKNearest(1);
		return m;
	}
	void findInBall(vector<T* > &res, int root, const Vec3f& pos, real radius2){
		KdNode* cur = &a[root];
		int first = aPos[cur->dim] > cur->split, second = !first;
		real dist = (cur->t->getPos() - aPos).L2();
		if (dist < radius2)
			res.push_back(cur->t);
		if ((cur->ch >> first) & 1){
			if (m < mLimit || a[root * 2 + first].b->minDist2(aPos) < radius2)
				findInBall(res, root * 2 + first, pos, radius2);
		}
		if ((cur->ch >> second) & 1){
			if (m < mLimit || a[root * 2 + second].b->minDist2(aPos) < radius2)
				findInBall(res, root * 2 + second, pos, radius2);
		}
	}

	~KdTree(){
		if (a != NULL)
			delete[] a;
		delete[] res;
	}
public:
	int root;
private:
	void _addToHeap(T* t){
		real dist = (t->getPos() - aPos).L2();
		if (m == mLimit){
			if (res[0].first < dist)
				return;
			pop_heap(res, res + m);
			res[m] = make_pair(dist, t);
			push_heap(res, res + m);
		}
		res[m ++] = make_pair(dist, t);
		if (m == mLimit)
			make_heap(res, res + m);
	}
	void _findKNearest(int root){
		KdNode* cur = &a[root];
		int first = aPos[cur->dim] > cur->split, second = !first;
		_addToHeap(cur->t);
		if ((cur->ch >> first) & 1){
			if (m < mLimit || a[root * 2 + first].b->minDist2(aPos) < res[0].first)
				_findKNearest(root * 2 + first);
		}
		if ((cur->ch >> second) & 1){
			if (m < mLimit || a[root * 2 + second].b->minDist2(aPos) < res[0].first)
				_findKNearest(root * 2 + second);
		}
	}

	int mDim;
	int mLimit;
	real mDist;
	int n, m;
	KdNode* a;
	vector<T* > mData;
	pair<real, T* > * res;
	Vec3f aPos;
};

// } TO be validate


//unbalanced KdTree using SAH

class KdTreeTri{
public:
	struct KdNode{
		KdNode(){
			isLeaf = 0;
			dim = -1;
			left = right = NULL;
			triangles.clear();
			b = NULL;
		}
		void naiveSplit();
		short isLeaf, dim;
		real split;
		Box* b;
		vector<Triangle*> triangles;
		KdNode* left;
		KdNode* right;
	};
	KdTreeTri(){
		clear();
	}
	void clear(){
		recurse = 0;
		n = 0;
		root = NULL;
		mData.clear();
	}
	void add(Triangle* x){
		mData.push_back(x);
	}
	void buildLeaf(KdNode*& root, const vector<Triangle* > a, int l, int r);
	void build(KdNode*& root, const vector<Triangle* > a, Box* Bbox, short lastDim = -1);
	void construct();

	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray){}

	void del(KdNode* root);
	~KdTreeTri(){
		del(root);
	}
public:
	static bool debug;
private:
	bool _traverse(KdNode *root, const Ray& ray, Intersection& isect);
	int mDim;
	int n;
	vector<Triangle* > mData;
	KdNode* root;
	//for debug
	int recurse;
	const int rec_limit = 1123456;
	const int MAX_KDTREE_LEAF_SIZE = 30;
};

}; // namespace Raytrace

#endif // CORE_KDTREE_H
