#include "trianglemesh.h"

namespace Raytracer {

TriangleMesh::TriangleMesh(string objFile, Material* aMaterial, Matrix44 transform){
	FILE* fp = fopen(objFile.c_str(), "r");
	mMaterial = aMaterial;
	mBoundingBox = new Box();
	//assert(fp != NULL);
	if (fp == NULL){
		printf("No such file: %s !", objFile.c_str());
		return;
	}
	progressMessage("start loading obj");
	bool fastIO = false; // fastIO with bug!!
	int lines = 0;
	if (fastIO){
		char* buffer = new char[11234567];
		char* ptr = buffer;
		int res = fread(buffer, 11234567, 1, fp);
		//printf("%d\n", res);
		// for (int i = 0; i < 1000 && buffer[i] >= 0; ++ i)
		// 	printf("%c", buffer[i]);
		for (char ch = getChar(ptr); ch > 0; ch = getChar(ptr)){
			while (ch > 0 && ch != '#' && ch != 'v' && ch != 'f')
				ch = getChar(ptr);
			if ((++ lines) % 10000 == 0)
				printf("line %d %c\n",lines, ch);
			if (ch == '#'){
				while (ch != '\n')
					ch = getChar(ptr);
			}else if (ch == 'v'){
				real x, y, z;
				readBuf(ptr, x), readBuf(ptr, y), readBuf(ptr, z);
				Vec3f pos(x, y, z);
				pos = (transform * pos).first;
				Vertex* vex = new Vertex(pos);
				mBoundingBox->update(pos);
				mVertexs.push_back(vex);
			}else if (ch == 'f'){
				int a, b, c;
				readBuf(ptr, a), readBuf(ptr, b), readBuf(ptr, c);
				a = chg(a), b = chg(b), c = chg(c);
	 			Triangle* tri = new Triangle(mVertexs, a, b, c);
	 			tri->setMaterial(mMaterial);
				mTriangles.push_back(tri);
			}
		}
		progressMessage("fastIO complete");
	}else{
		char type[5];
		char buf[1005];
		while (fscanf(fp, "%s", type)!=EOF){
			if ((++ lines) % 10000 == 0)
				printf("line %d %c\n",lines, type[0]);
			if (type[0] == '#')
				fgets(buf, sizeof(buf), fp);
			else if (type[0] == 'v'){
				real x, y, z;
				fscanf(fp, "%lf %lf %lf", &x, &y, &z);
				Vec3f pos(x, y, z);
				pos = (transform * pos).first;
				Vertex* vex = new Vertex(pos);
				//vex.prt();
				mBoundingBox->update(pos);
				mVertexs.push_back(vex);
			}else if (type[0] == 'f'){
				int a, b, c;
				fscanf(fp, "%d %d %d", &a, &b, &c);
				a = chg(a), b = chg(b), c = chg(c);
				// printf("%d %d %d\n",a,b,c);
				// mVertexs[a].prt();
				// mVertexs[b].prt();
				// mVertexs[c].prt();
	 			Triangle* tri = new Triangle(mVertexs, a, b, c);
	 			tri->setMaterial(mMaterial);
				mTriangles.push_back(tri);
				//tri->mNorm.prt();
			}
		}
	}
	mBoundingBox->prt();
	progressMessage("done loading");
}

void TriangleMesh::dump(string file){
	file = "test_data/dump/" + file;
	FILE* fp = fopen(file.c_str(), "w");
	fprintf(fp, "# dumped\n");
	for (Vertex* vex : mVertexs)
		vex->prt(fp);
	for (Triangle* tri : mTriangles)
		tri->prt(fp);
	fclose(fp);
}

bool TriangleMesh::intersect(const Ray& ray, Intersection& isect){
	int retval = MISS;
	for (Triangle* triangle : mTriangles){
		if (triangle->intersect(ray, isect) == HIT)
			retval = HIT;
	}
	return retval;
}

TriangleMesh::~TriangleMesh(){
	delete mBoundingBox;
	for (Vertex* vex : mVertexs)
		delete vex;
	for (Triangle* tri : mTriangles)
		delete tri;
}

}; // namespace Raytracer
