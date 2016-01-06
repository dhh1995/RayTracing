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
		short ch; // child 0 : none , 1 : left , 2 : right , 3 : both
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
		//x->prt();
		++ n;
		mData.push_back(x);
	}
	int getN(){
		return n;
	}
	int getRoot(){
		return root;
	}
	void build(int root, int l, int r){
		assert(l < r);
		KdNode* cur = &a[root];
		cur->b = new Box;
		for (int i = l; i < r; ++ i)
			cur->b->update(mData[i]->getPos());
		cur->dim = mDim = cur->b->argMaxDiff();
		//printf("%d %d %d %d\n", root, l, r, mDim);
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
		assert(n > 0);
		//printf("%d\n",n);
		int N = n * 2;

		if (a != NULL)
			delete[] a;
		a = new KdNode[N];
		root = 1;
		build(1, 0, n);
	}
	pair<real, T* > * getRes(){
		return res;
	}
	pair<real, T* > getRes(int k){
		return res[k];
	}
	int getKNearest(const Vec3f& pos, int K, real maxDist = INF){
		//colorMessage("New Request !!", 5);
		m = 0, mLimit = K;
		aPos = pos;
		mDist = maxDist * maxDist;
		_findKNearest(root);
		//_bruteForce();
		make_heap(res, res + m);
		//if (m > 0)
		//	sort(res, res + m);
		return m;
	}
	void findInBall(vector<T* > &res, int root, const Vec3f& pos, real radius2){
		//use res.push_back, can be merged with getKNearst
		KdNode* cur = &a[root];
		int first = pos[cur->dim] > cur->split, second = !first;
		real dist = (cur->t->getPos() - pos).L2();
		if (dist < radius2)
			res.push_back(cur->t);
		if ((cur->ch >> first) & 1){
			if (a[root * 2 + first].b->minDist2(pos) < radius2)
				findInBall(res, root * 2 + first, pos, radius2);
		}
		if ((cur->ch >> second) & 1){
			if (a[root * 2 + second].b->minDist2(pos) < radius2)
				findInBall(res, root * 2 + second, pos, radius2);
		}
	}
	vector<T*>& getData(){
		return mData;
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
		if (dist > mDist)
			return;
		if (m == mLimit){
			if (res[0].first < dist)
				return;
			pop_heap(res, res + m);
			res[m - 1] = make_pair(dist, t);
			push_heap(res, res + m);
		}else{
			res[m ++] = make_pair(dist, t);
			if (m == mLimit)
				make_heap(res, res + m);
		}
	}
	void _findKNearest(int root){
		KdNode* cur = &a[root];
		int first = aPos[cur->dim] > cur->split, second = !first;
		//aPos.prt();
		//printf("%d %d %lf\n", first, cur->dim, cur->split);
		_addToHeap(cur->t);
		if ((cur->ch >> first) & 1){
			//colorMessage("Step into first", 4);
			real minDist2 = a[root * 2 + first].b->minDist2(aPos);
			if (minDist2 < mDist && (m < mLimit || minDist2 < res[0].first))
				_findKNearest(root * 2 + first);
		}
		if ((cur->ch >> second) & 1){
			//colorMessage("Step into second", 4);
			real minDist2 = a[root * 2 + second].b->minDist2(aPos);
			if (minDist2 < mDist && (m < mLimit || minDist2 < res[0].first))
				_findKNearest(root * 2 + second);
		}
		//colorMessage("Step out", 4);
	}
	void _bruteForce(){
		for (int i = 0; i < n; ++ i)
			res[i] = make_pair((mData[i]->getPos() - aPos).L2(), mData[i]);
			//res[i]  = make_pair((a[i].t->getPos() - aPos).L2(), a[i].t);
		sort(res, res + n);
		m = 0;
		while (m < mLimit && res[m].first < mDist)
			++ m;
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

}; // namespace Raytracer

#endif // CORE_KDTREE_H
