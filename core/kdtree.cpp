#include "kdtree.h"

namespace Raytracer {


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

void KdTreeTri::build(KdNode*& root, const vector<Triangle* >& a, Box* Bbox, short lastDim){
	//printf("%d %d %d\n",recurse, a.size());
	if (++ recurse > rec_limit)
		return;
	if (a.empty())
		return;
	if (root == NULL)
		root = new KdNode();
	int m = a.size();
	if (m <= MAX_KDTREE_LEAF_SIZE){
		root->triangles = a;
		root->isLeaf = true;
		return;
	}
	if (debug){
		printf("recurse %d\n", recurse);
		printf("building, size = %d\n",m);
	}

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

bool KdTreeTri::_traverse(KdNode *root, const Ray& ray, Intersection& isect){
	if (root == NULL)
		return MISS;
	//puts("!");
	bool res = MISS;
	if (root->isLeaf){
		if (debug){
			puts("is Leaf");
			for (Triangle* tri : root->triangles)
				tri->prt();
		}
		res = MISS;
		for (Triangle* tri : root->triangles)
			if (tri->intersect(ray, isect) == HIT)
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
	if (_traverse(first, ray, isect) == HIT){
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
	if (!ignore && _traverse(second, ray, isect) == HIT){
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
	return _traverse(root, ray, isect);
}

void KdTreeTri::del(KdNode* root){
	if (root != NULL){
		del(root->left);
		del(root->right);
		delete root;
	}
}

}; // namespace Raytracer
