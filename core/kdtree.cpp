#include "kdtree.h"

namespace Raytracer {

//TO be validate {

int KdTree::mDim;

bool PhotonCmp(const Photon* A, const Photon* B){
	return A->getAxis(KdTree::mDim) < B->getAxis(KdTree::mDim);
}

void KdTree::build(int root, int l, int r){
	KdNode* cur = &a[root];
	cur->b = new Box;
	for (int i = l; i < r; ++ i)
		cur->b->update(mData[i]->getPos());
	cur->dim = mDim = cur->b->argMaxDiff();
	int m = (l + r) / 2;
	nth_element(mData.begin() + l, mData.begin() + m, mData.begin() + r, PhotonCmp);
	cur->t = mData[m];
	cur->split = cur->t->getAxis(cur->dim);
	cur->ch = 0;
	if (l < m)
		build(root * 2, l, m), cur->ch |= 1;
	if (m + 1 < r)
		build(root * 2 + 1, m + 1, r), cur->ch |= 2;
}

void KdTree::addToHeap(Photon* t){
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

void KdTree::findKNearest(int root){
	KdNode* cur = &a[root];
	int first = aPos[cur->dim] > cur->split, second = !first;
	addToHeap(cur->t);
	if ((cur->ch >> first) & 1){
		if (m < mLimit || a[root * 2 + first].b->minDist(aPos) < res[0].first)
			findKNearest(root * 2 + first);
	}
	if ((cur->ch >> second) & 1){
		if (m < mLimit || a[root * 2 + second].b->minDist(aPos) < res[0].first)
			findKNearest(root * 2 + second);
	}
}

int KdTree::getKNearest(const Vec3f& pos, int K){
	m = 0, mLimit = K;
	aPos = pos;
	findKNearest(1);
	return m;
}
// } TO be validate


//KdTree for Triangle*

bool KdTreeTri::debug;

void KdTreeTri::naiveSplit(Box *A, short &dim, real &split){
	if (dim == -1)
		dim = A->argMaxDiff();
	else
		if (++ dim > 2)
			dim = 0;
	split = A->getMid(dim);
}

void KdTreeTri::buildLeaf(KdNode *root, const vector<Triangle* > a, int l, int r){
	if (++ recurse > rec_limit)
		return;
	if (root == NULL)
		root = new KdNode();
	root->isLeaf = r - l;
	if (debug){
		printf("recurse %d\n", recurse);
		printf("build in leaf, size = %d\n", r - l);
	}
	int m = (l + r) / 2;
	root->x = a[m];
	if (l < m)
		buildLeaf(root->left, a, l, m);
	if (m + 1 < r)
		buildLeaf(root->right, a, m+1, r);
}

void KdTreeTri::build(KdNode* root, const vector<Triangle* > a, short lastDim){
	if (++ recurse > rec_limit)
		return;
	if (a.empty())
		return;
	int m = a.size();
	if (m <= MAX_KDTREE_LEAF_SIZE){
		buildLeaf(root, a, 0, m);
		return;
	}
	if (debug){
		printf("recurse %d\n", recurse);
		printf("building, size = %d\n",m);
	}
	if (root == NULL)
		root = new KdNode();

	root->dim = lastDim;
	Box* b = new Box;
	for (Triangle* i : a)
		b->update(i->getBBox());
	naiveSplit(b, root->dim, root->split);

	vector<Triangle* > aL, aR;
	for (Triangle* i : a){
		int res = i->getSide(root->dim, root->split);
		if (res <= 0)
			aL.push_back(i);
		if (res >= 0)
			aR.push_back(i);
	}
	build(root->left, aL, root->dim);
	build(root->right, aR, root->dim);
}

void KdTreeTri::construct(){
	build(root, mData);
}

bool KdTreeTri::traverse(KdNode *root, const Ray& ray, Intersection& isect){
	if (root == NULL)
		return MISS;
	if (root->isLeaf){
		traverse(root->left, ray, isect);
		traverse(root->left, ray, isect);
		return MISS;
	}
	return 0;
}

bool KdTreeTri::intersect(const Ray& ray, Intersection& isect){
	traverse(root, ray, isect);
}

void KdTreeTri::del(KdNode* root){
	if (root != NULL){
		del(root->left);
		del(root->right);
		delete root;
	}
}

}; // namespace Raytracer
