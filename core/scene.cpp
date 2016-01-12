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

bool Scene::loadObj(string folder, string fileName){
	string path = folder + "/" + fileName + ".obj";
	FILE* fp = fopen(path.c_str(), "r");
	if (fp == NULL)
		return false;

	vector<Vertex* > vertexs;
	Box* boundingBox = new Box;
	map<string, Material*> materials;
	Material* currentMaterial = NULL;
	Material* defaultMtl = new Material(WHITE, 0, 0, WHITE, BLACK, 1, WHITE / 5);
	int lines = 0;
	char tmp[105];
	char buf[1005];
	while (fscanf(fp, "%s", tmp)!=EOF){
		puts(tmp);
		string type = tmp;
		if ((++ lines) % 10000 == 0)
			printf("line %d %s\n",lines, tmp);
		if (type[0] == '#')
			fgets(buf, sizeof(buf), fp);
		else if (type == "mtllib"){
			fscanf(fp, "%s", tmp);
			string path = tmp;
			path = folder + "/" + path;
			FILE* mtllib = fopen(path.c_str(), "r");
			if (mtllib == NULL){
				colorMessage("Mtllib not find", 1);
				continue;
			}
			Material* mtl;
			string name;
			real r,g,b;
			while (fscanf(mtllib, "%s", tmp) != EOF){
				string type = tmp;
				if (type == "newmtl"){
					if (mtl != NULL)
						materials[name] = mtl;
					mtl = new Material();
					fscanf(mtllib, "%s", tmp);
					name = tmp;
				}else if (type == "Ka"){
				}else if (type == "Kd"){
					fscanf(mtllib, "%lf %lf %lf", &r, &g, &b);
					mtl->setDiffuse(Color(r,g,b));
				}else if (type == "Ks"){
					fscanf(mtllib, "%lf %lf %lf", &r, &g, &b);
					mtl->setSpecular(Color(r,g,b));
				}else if (type == "Ni"){
					real index;
					fscanf(mtllib, "%lf", &index);
					mtl->setRefrIndex(index);
				}
			}
			if (mtl != NULL)
				materials[name] = mtl;
			//convert material;
		}else if (type == "v"){
			real x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			Vec3f pos(x, y, z);
			Vertex* vex = new Vertex(pos);
			boundingBox->update(pos);
			// printf("%lf %lf %lf\n",x,y,z);
			vertexs.push_back(vex);
		}else if (type == "f"){
			int n = vertexs.size();
			int a, b, c;
			fscanf(fp, "%d %d %d", &a, &b, &c);
			a = chg(n, a), b = chg(n, b), c = chg(n, c);
			// printf("%d %d %d\n",a,b,c);
 			Triangle* tri = new Triangle(vertexs, a, b, c);
 			assert(currentMaterial != NULL);
 			tri->setMaterial(currentMaterial);
 			mAggregate.add(tri);
		}else if (type == "usemtl"){
			fscanf(fp, "%s", tmp);
			string materialName = tmp;
			currentMaterial = materials[materialName];
			if (currentMaterial == NULL)
				currentMaterial = defaultMtl;
		}
	}
	return true;
}

}; // namespace Raytracer