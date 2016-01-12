#include "scene.h"

namespace Raytracer {

bool Scene::intersect(const Ray& ray, Intersection& isect, bool ignoreLight){
	isect.setDist(INF);
	bool ans = mAggregate.intersect(ray, isect);
	for (Primitive* obj : mPrimitives)
		if (obj->intersect(ray, isect) == HIT)
			ans = HIT;

	if (!ignoreLight)
		for (Light* obj : mLights)
			if (obj->intersect(ray, isect) == HIT)
				ans = HIT;
	return ans;
}

bool Scene::intersectP(const Ray& ray){
	// int ans = mAggregate->intersectP(ray);
	// for (Primitive* obj : mPrimitives)
	// 	if (obj->intersectP(ray) != 0)
	// 		return 1;
	return MISS;
}

real Scene::visible(const Ray& ray, real dist){
	Intersection isect;
	if (intersect(ray, isect, true) == HIT)
		if (isect.getDist() + EPS < dist)
			return 0.0f;
	return 1.0f;
}

bool Scene::loadObj(string path){
	FILE* fp = fopen(path.c_str(), "r");
	if (fp == NULL)
		return false;

	vector<Vertex* > vertexs;
	Box* boundingBox;
	map<string, Material*> materials;
	Material* currentMaterial = NULL;

	int lines = 0;
	char tmp[105];
	char buf[1005];
	while (fscanf(fp, "%s", tmp)!=EOF){
		string type = tmp;
		if ((++ lines) % 10000 == 0)
			printf("line %d %s\n",lines, tmp);
		if (type == "#")
			fgets(buf, sizeof(buf), fp);
		else if (type == "matlib"){
			fscanf(fp, "%s", tmp);
			string matlibPath = tmp;
			//convert material;
		}else if (type == "v"){
			real x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			Vec3f pos(x, y, z);
			Vertex* vex = new Vertex(pos);
			boundingBox->update(pos);
			vertexs.push_back(vex);
		}else if (type == "f"){
			int n = vertexs.size();
			int a, b, c;
			fscanf(fp, "%d %d %d", &a, &b, &c);
			a = chg(n, a), b = chg(n, b), c = chg(n, c);
			// printf("%d %d %d\n",a,b,c);
			// mVertexs[a].prt();
			// mVertexs[b].prt();
			// mVertexs[c].prt();
 			Triangle* tri = new Triangle(vertexs, a, b, c);
 			assert(currentMaterial != NULL);
 			tri->setMaterial(currentMaterial);
 			mAggregate.add(tri);
		}else if (type == "usemtl"){
			fscanf(fp, "%s", tmp);
			string materialName = tmp;
			currentMaterial = materials[materialName];
		}
	}
	return true;
}

}; // namespace Raytracer