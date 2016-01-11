#include "scene.h"

namespace Raytracer {

bool Scene::intersect(const Ray& ray, Intersection& isect){
	isect.setDist(INF);
	bool ans = mAggregate.intersect(ray, isect);
	for (Primitive* obj : mPrimitives){
		if (obj->intersect(ray, isect) == HIT)
			ans = HIT;
	}
	return ans;
}

bool Scene::intersectP(const Ray& ray){
	// int ans = mAggregate->intersectP(ray);
	// for (Primitive* obj : mPrimitives)
	// 	if (obj->intersectP(ray) != 0)
	// 		return 1;
	return MISS;
}

real Scene::calcShade(Light* light, Vec3f pos, Vec3f& dir){
	Vec3f center = light->getPos();
	dir = center - pos;
	if (light->getType() == "Point"){
		real dist = dir.length();
		dir /= dist;
		//printf("%lf\n",dist);
		Intersection isect;
		if (intersect(Ray(center, -dir), isect) == HIT){
			// center.prt();
			// pos.prt();
			// printf("%lf\n",isect.getDist());
			// isect.getPos().prt();
			if (isect.getDist() + EPS < dist)
				return 0.;
		}
		return 1.;
	}

	real res = 1;
	int n = 3, n2 = n / 2;
	for (int i=0;i<n;++i)
		for (int j=0;j<n;++j){
			Vec3f newCenter = light->getPos((i-n2) / 2. , (j-n2) / 2.);
			Vec3f d = newCenter - pos;
			//light->getPos((i-n2) / 2. , (j-n2) / 2.).prt();
			real dist = d.length();
			d /= dist;
			Intersection isect;
			if (intersect(Ray(newCenter, -d), isect) == HIT)
				if (isect.getDist() + EPS < dist)
					res -= 1. / n / n;
		}
	dir.Normalize();
	return res;
}

Color Scene::getLi(const Ray& ray, const Intersection& isect){
	Vec3f pi = isect.getPos();
	Vec3f N = isect.getNorm();
	Color color = isect.getColor();
	Material* matter = isect.getPrim()->getMaterial();

	Color res = mAmbient * matter->getKa();
	for (Light* light : mLights){
		Vec3f L;
		real shade = calcShade(light, pi, L);
		if (shade > 0){
			// calculate diffuse shading
			real diff = matter->getDiffuse();
			if (diff > 0){
				real dt = dot(L, N);
				if (dt > 0){
					real diffuse = dt * diff * shade;
					// add diffuse component to ray color
					res += diffuse * color * light->getColor();
				}
			}
			// determine specular component using Schlick's BRDF approximation
			real spec = matter->getSpecular();
			if (spec > 0){
				// point light source: sample once for specular highlight
				Vec3f R = L - 2.0f * dot( L, N ) * N;
				real dt = dot(ray.d, R);	
				if (dt > 0){
					real specular = dt * spec * shade / (50 - 50 * dt + dt);
					// add specular component to ray color
					res += specular * light->getColor();
				}
			}
		}
	}
	return res;
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