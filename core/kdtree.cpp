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

void KdTreeTri::KdNode::naiveSplit(){
	if (dim == -1)
		dim = b->argMaxDiff();
	else
		if (++ dim > 2)
			dim = 0;
	split = b->getMid(dim);
}

void KdTreeTri::buildLeaf(KdNode*& root, const vector<Triangle* > a, int l, int r){
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

void KdTreeTri::build(KdNode*& root, const vector<Triangle* > a, Box* Bbox, short lastDim){
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
	root->b = Bbox;
	//for (Triangle* i : a)
	//	root->b->update(i->getBBox());
	//root->b->prt();
	root->naiveSplit();
	// printf("%d %lf\n", root->dim, root->split);
	// root->b->split(root->dim, 0, root->split)->prt();

	vector<Triangle* > aL, aR;
	for (Triangle* i : a){
		int res = i->getSide(root->dim, root->split);
		// if (debug){
		// 	printf("res = %d dim = %d split = %lf\n", res, root->dim, root->split);
		// 	i->prt();
		// }
		if (res <= 0)
			aL.push_back(i);
		if (res >= 0)
			aR.push_back(i);
	}
	// build(root->left, aL, root->b, root->dim);
	// build(root->right, aR, root->b, root->dim);
	build(root->left, aL, root->b->split(root->dim, 0, root->split), root->dim);
	build(root->right, aR, root->b->split(root->dim, 1, root->split), root->dim);
}

void KdTreeTri::construct(){
	Box* Bbox = new Box;
	for (Triangle* i : mData)
		Bbox->update(i->getBBox());
	build(root, mData, Bbox);
}

bool KdTreeTri::traverse(KdNode *root, const Ray& ray, Intersection& isect){
	if (root == NULL)
		return MISS;
	//puts("!");
	bool res = MISS;
	if (root->isLeaf){
		if (debug){
			puts("in Leaf");
			root->x->prt();
		}
		res = root->x->intersect(ray, isect);
		
		if (traverse(root->left, ray, isect) == HIT)
			res = HIT;
		if (traverse(root->right, ray, isect) == HIT)
			res = HIT;
		return res;
	}
	KdNode* first = root->left;
	KdNode* second = root->right;

	real near = -1, far = -1;
	int dim = root->dim;
	root->b->getNearFar(ray, near, far);
	if (debug){
		ray.prt();
		root->b->prt();
		printf("dim = %d split = %lf\n",root->dim, root->split);
		printf("near = %lf far = %lf\n", near, far);
	}
	if (near > far || far < -1)
		return MISS;
	real split = root->split;//(root->split - ray.o[dim]) / ray.d[dim];
	real nearX = ray(near)[dim];
	real farX  = ray(far)[dim];
	bool ignore = false, swaped = false;
	if (nearX > split){
		swap(first, second);
		swaped = true;
		if (farX > split)
			ignore = true;
	}
	if (nearX < split && farX < split)
		ignore = true;
	//TODO
	if (debug){
		ray.prt();
		printf("near = %lf far = %lf\n", near, far);
		printf("nearX = %lf farX = %lf\n", nearX, farX);
		printf("first is left %d\n", first == root->left);
		colorMessage("step into first child", 5);
	}
	double tmpDist = isect.getDist();
	if (traverse(first, ray, isect) == HIT){
		//printf("%d %lf\n", dim, root->split);
		//printf("!dist = %lf split = %lf\n", isect.getDist(), split);
		//colorMessage("Important", 1);
		//isect.getPos().prt();
		//printf("%d %lf | %d %d\n", dim, split, !swaped && isect.getPos()[dim] < split + EPS, swaped && isect.getPos()[dim] > split - EPS);
		if (ignore)
			return HIT;
		if (!swaped && isect.getPos()[dim] < split + EPS)
			return HIT;
		if (swaped && isect.getPos()[dim] > split - EPS)
			return HIT;
		isect.setDist(tmpDist);
	}
	if (debug)
		colorMessage("step into second child", 5);
	if (!ignore && traverse(second, ray, isect) == HIT){
		// if (near < 0 && far > 0 && nearX < split && split < farX){
		// 	printf("near = %lf far = %lf\n", near, far);
		// }
		//printf("%d %lf\n", dim, root->split);
		//printf("!dist = %lf far = %lf \n", isect.getDist(), far);
		if (isect.getDist() < far + EPS)
			return HIT;
		isect.setDist(tmpDist);
	}
	return MISS;
}

bool KdTreeTri::intersect(const Ray& ray, Intersection& isect){
	return traverse(root, ray, isect);
}

void KdTreeTri::del(KdNode* root){
	if (root != NULL){
		del(root->left);
		del(root->right);
		delete root;
	}
}

}; // namespace Raytracer
