#include "scene.h"

namespace Raytracer {

bool Scene::intersect(const Ray& ray, Intersection& isect, bool useLight){
	isect.setDist(INF);
	bool ans = mAggregate.intersect(ray, isect);
	for (Primitive* obj : mPrimitives)
		if (obj->intersect(ray, isect) == HIT)
			ans = HIT;

	if (useLight)
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
	vector<UV> vt;
	vector<Vec3f> vn;
	Box* boundingBox = new Box;
	map<string, Material*> materials;
	Material* currentMaterial = NULL;
	Material* defaultMtl = new Material(WHITE / 5, WHITE, BLACK);
	int lines = 0;
	char tmp[1005];
	char buf[1005];
	while (fscanf(fp, "%s", tmp)!=EOF){
		// puts(tmp);
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
					fscanf(mtllib, "%lf %lf %lf", &r, &g, &b);
					mtl->setAmbient(Color(r,g,b));
				}else if (type == "Kd"){
					fscanf(mtllib, "%lf %lf %lf", &r, &g, &b);
					mtl->setDiffuse(Color(r,g,b));
				}else if (type == "Ks"){
					fscanf(mtllib, "%lf %lf %lf", &r, &g, &b);
					mtl->setSpecular(Color(r,g,b));
				}else if (type == "Ni"){
					real index;
					fscanf(mtllib, "%lf", &index);
					if (index != 1.0f)
						mtl->setFresnel(new Fresnel(index));
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
		}else if (type == "vt"){
			real u, v;
			fscanf(fp, "%lf %lf", &u, &v);
			// printf("%lf %lf %lf\n",x,y,z);
			vt.push_back(make_pair(u,v));
		}else if (type == "vn"){
			real x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			vn.push_back(Vec3f(x, y, z));
		}else if (type == "f"){
			char blank;
			int n = vertexs.size();
			int a, b, c, d;
			int va, vb, vc, vd;
			int na, nb, nc, nd;
			fgets(tmp, 1000, fp);
			char* ptr = tmp;
			char ch;

			bool debug = false;
			if ((ch = readBuf(ptr, a)) =='/')
				if ((ch = readBuf(ptr, va)) =='/') 
					ch = readBuf(ptr, na);
			if ((ch = readBuf(ptr, b)) =='/')
				if ((ch = readBuf(ptr, vb)) =='/')
					ch = readBuf(ptr, nb);
			if ((ch = readBuf(ptr, c)) =='/')
				if ((ch = readBuf(ptr, vc)) =='/')
					ch = readBuf(ptr, nc);
			a = chg(n, a), b = chg(n, b), c = chg(n, c);
 			Triangle* tri = new Triangle(vertexs, a, b, c);
 			assert(currentMaterial != NULL);
 			tri->setMaterial(currentMaterial);
 			mAggregate.add(tri);

			if (ch == ' '){
				if ((ch = readBuf(ptr, d)) =='/')
					if ((ch = readBuf(ptr, vd)) =='/')
						readBuf(ptr, nd);
				d = chg(n, d);
				tri = new Triangle(vertexs, a, c, d);
	 			assert(currentMaterial != NULL);
	 			tri->setMaterial(currentMaterial);
	 			mAggregate.add(tri);
	 		}
		}else if (type == "usemtl"){
			fscanf(fp, "%s", tmp);
			string materialName = tmp;
			currentMaterial = materials[materialName];
			if (currentMaterial == NULL)
				currentMaterial = defaultMtl;
		}else if (type == "s" || type == "g")
			fgets(tmp, 1000, fp);
	}
	return true;
}

}; // namespace Raytracer