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
	if (l + 1 < m)
		build(root * 2, l, m), cur->ch |= 1;
	if (m + 2 < r)
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


void KdTreeTri::construct(){


}

}; // namespace Raytracer
