#include "trianglemesh.h"

namespace Raytracer {

void Triangle::setIsect(Intersection& isect, real dist, Vec3f pos, bool backSide){
	isect.setPrim(this);
	isect.setDist(dist);
	isect.setPos(pos);
	Vec3f norm = mNorm;
	if (backSide)
		norm = -norm;
	if (mMaterial->haveTexture()){
		//TODO
	}else{
		isect.setNorm(norm);
		isect.setColor(mMaterial->getColor());
	}
}

// int Triangle::intersect(const Ray& ray, Intersection& isect){
// 	Vec3f Rd = ray.d;
// 	Vec3f S = A - ray.o;
// 	real res = det(Rd, E1, E2);
// 	if (abs(res) < EPS)
// 		return MISS;
// 	Vec3f ans = Vec3f(det(S, E1, E2), det(Rd, S, E2), det(Rd, E1, S)) / res;
// 	if (ans.x < 0 || ans.y < 0 ||  ans.z < 0 || ans.y + ans.z > 1)
// 		return MISS;
// 	isect.setDist(ans.x);
// 	isect.setPos(ray(ans.x));
// 	return HIT;
// }

bool Triangle::inside(const Vec3f pos){
	return dot(cross(B - A, pos - A), mNorm) >= 0
		&& dot(cross(C - B, pos - B), mNorm) >= 0
		&& dot(cross(A - C, pos - C), mNorm) >= 0;
}

int Triangle::intersect(const Ray& ray, Intersection& isect){
	//puts("inside Triangle");
	float d = dot(mNorm, ray.d);
	if (d != 0){
		float dist = -(dot(mNorm, ray.o) + mD) / d;
		//mNorm.prt();
		//printf("%lf d=%lf\n",dist, d);
		if (dist > 0){
			if (dist < isect.getDist()) {
				Vec3f pos = ray(dist);
				if (!inside(pos))
					return MISS;
				setIsect(isect, dist, pos, dot(ray.d, mNorm) > 0);
				return HIT;
			}
		}
	}
	return MISS;
}

int Triangle::intersectP(const Ray& ray){

}

TriangleMesh::TriangleMesh(string objFile, Material* aMaterial, Vec3f trans){
	FILE* fp = fopen(objFile.c_str(), "r");
	mMaterial = aMaterial;
	//assert(fp != NULL);
	if (fp == NULL){
		printf("No such file: %s !", objFile.c_str());
		return;
	}
	puts("start loading");
	char type[5];
	char buf[1005];
	while (fscanf(fp, "%s", type)!=EOF){
		if (type[0] == '#')
			fgets(buf, sizeof(buf), fp);
		else if (type[0] == 'v'){
			real x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			Vec3f vex(x, y, z);
			//vex.prt();
			vex += trans;
			mVertexs.push_back(vex);
		}else if (type[0] == 'f'){
			int a, b, c;
			fscanf(fp, "%d %d %d",&a, &b, &c);
			a = chg(a), b = chg(b), c = chg(c);
			// printf("%d %d %d\n",a,b,c);
			// mVertexs[a].prt();
			// mVertexs[b].prt();
			// mVertexs[c].prt();
 			Triangle* tri = new Triangle(mVertexs[a], mVertexs[b], mVertexs[c]);
 			tri->setMaterial(mMaterial);
			mTriangles.push_back(tri);
			//tri->mNorm.prt();
		}
	}
	puts("done loading");
}

int TriangleMesh::intersect(const Ray& ray, Intersection& isect){
	Intersection tmp;
	isect.setDist(INF);
	int retval = MISS;
	for (Triangle* triangle : mTriangles){
		if (triangle->intersect(ray, tmp) != 0){
			retval = HIT;
			if (tmp.getDist() < isect.getDist())
				isect = tmp;
		}
	}
	return retval;
}

}; // namespace Raytrace
