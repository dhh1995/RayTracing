#include "trianglemesh.h"

namespace Raytracer {

TriangleMesh::TriangleMesh(string objFile, Material* aMaterial, Vec3f trans, real scale){
	FILE* fp = fopen(objFile.c_str(), "r");
	mMaterial = aMaterial;
	mBoundingBox = new Box();
	//assert(fp != NULL);
	if (fp == NULL){
		printf("No such file: %s !", objFile.c_str());
		return;
	}
	progressMessage("start loading obj");
	char type[5];
	char buf[1005];
	while (fscanf(fp, "%s", type)!=EOF){
		if (type[0] == '#')
			fgets(buf, sizeof(buf), fp);
		else if (type[0] == 'v'){
			real x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			Vec3f pos(x, y, z);
			pos += trans;
			pos *= scale;
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
	mBoundingBox->prt();
	progressMessage("done loading");
}

void TriangleMesh::contraction(VertexPair P){

}

void TriangleMesh::decimation(real percent, real threshold){
	int m = mTriangles.size(), need = int(m * percent);
	int n = 0;
	for (Vertex* vex : mVertexs){
		MeshVertex* meshVex = new MeshVertex(*vex, n ++);
		mVexCloud.add(meshVex);
	}
	vector<MeshVertex*> meshVertexs = mVexCloud.getData();
	mVexCloud.construct();
	for (Triangle* tri : mTriangles){
		Matrix44 quadMatrix(tri->getPlaneParam());
		for (int i = 0; i < 3; ++ i)
			meshVertexs[tri->getVexID(i)]->merge(quadMatrix);
	}
	for (MeshVertex* vex : meshVertexs){
		vector<MeshVertex* > res;
		mVexCloud.findInBall(res, mVexCloud.root, vex->getPos(), threshold * threshold);
		for (MeshVertex* near : res){

		}
	}
	while (!Q.empty() && n > need){
		VertexPair pair = Q.top();
		Q.pop();

	}
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

}; // namespace Raytrace
