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
	KdTree(int MaxK = MAX_K_NEAREST){
		a = NULL;
		res = new pair<real, Photon* >[MaxK];
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

		if (a != NULL)
			delete[] a;
		a = new KdNode[N];

		build(1, 0, n);
	}
	pair<real, Photon* > getKthPhoton(int k){
		return res[k];
	}
	int getKNearest(const Vec3f& pos, int K);
	~KdTree(){
		if (a != NULL)
			delete[] a;
		delete[] res;
	}
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
	pair<real, Photon* > * res;
	Vec3f aPos;
};

//unbalanced KdTree using SAH

class KdTreeTri{
public:
	struct KdNode{
		KdNode(){
			isLeaf = 0;
			dim = -1;
			left = right = NULL;
			x = NULL;
			b = NULL;
		}
		void naiveSplit();
		short isLeaf, dim;
		real split;
		Box* b;
		Triangle* x;
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

	bool traverse(KdNode *root, const Ray& ray, Intersection& isect);
	bool intersect(const Ray& ray, Intersection& isect);
	bool intersectP(const Ray& ray){}

	void del(KdNode* root);
	~KdTreeTri(){
		del(root);
	}
public:
	static bool debug;
private:
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
