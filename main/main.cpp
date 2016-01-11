#include <sys/resource.h>

#include "core/common.h"
//#include "obj_parser/SimpleObject.h"
#include "core/args.h"
#include "core/camera.h"
#include "core/scene.h"
#include "core/light.h"
#include "core/reflection.h"
#include "renderers/testrenderer.h"
#include "renderers/photonrenderer.h"
#include "film/image.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/trianglemesh.h"
#include "decimation/demesh.h"

using namespace Raytracer;
using namespace Decimation;

DeMesh* runDecimation(string file, Material* mat, string dumpName, const Args& args){
	char name[55];
	DeMesh* obj = new DeMesh(file, mat);
	int m = obj->getTriangles().size();
	int need = args.needTriangle;
	if (args.needOption == "ratio")
		need = int(m * args.needRatio);
	real threshold = args.threshold;
	if (threshold < 0)
		threshold = obj->diagonalLength() / (-threshold);
	progressMessage("start decimation");
	obj->decimation(need, threshold);

	sprintf(name, "%s_res_%d.obj", dumpName.c_str(), need);
	obj->dump(name);
	progressMessage("end dumping");

	return obj;
}

void DecimationTask(const Args& args)
{
	enum OBJ{
		ARMA,
		BLOCK,
		BUDDHA,
		BUNNY,
		CUBE,
		DINASAUR,
		DRAGON,
		FANDISK,
		HORSE,
		KITTEN,
		ROCKER_ARM,
		SPHERE,
	};
	int testModel = args.useModel;
	DeMesh *resObj;

	progressMessage("start loading");
	switch(testModel){
	case ARMA:
		resObj = runDecimation("test_data/Arma.obj", NULL, "arma", args);
		break;
	case BLOCK:
		resObj = runDecimation("test_data/block.obj", NULL, "block", args);
		break;
	case BUDDHA:
		resObj = runDecimation("test_data/Buddha.obj", NULL, "buddha", args);
		break;
	case BUNNY:
		resObj = runDecimation("test_data/bunny.fine.obj", NULL, "bunny", args);
		break;
	case CUBE:
		resObj = runDecimation("test_data/cube.obj", NULL, "cube", args);
		break;
	case DINASAUR:
		resObj = runDecimation("test_data/dinosaur.2k.obj", NULL, "dinosaur", args);
		break;
	case DRAGON:
		resObj = runDecimation("test_data/fixed.perfect.dragon.100K.0.07.obj", NULL, "dragon", args);
		break;
	case FANDISK:
		resObj = runDecimation("test_data/fandisk.18k.obj", NULL, "fandisk", args);
		break;
	case HORSE:
		resObj = runDecimation("test_data/horse.fine.90k.obj", NULL, "horse", args);
		break;
	case KITTEN:
		resObj = runDecimation("test_data/kitten.50k.obj", NULL, "kitten", args);
		break;
	case ROCKER_ARM:
		resObj = runDecimation("test_data/rocker-arm.18k.obj", NULL, "rocker-arm", args);
		break;
	case SPHERE:
		resObj = runDecimation("test_data/sphere.obj", NULL, "sphere", args);
		break;
	}
}

void emitDebugRay(Renderer* renderer, Ray ray){

// ray = Ray(Vec3f(-20, 0, 0), Vec3f(1,0,0));
// for (int i = -10; i <= 0; ++ i)
// for (int j = 0; j <= 10 ; ++ j){
// 	ray.setDir(Vec3f(1, i * 0.01, j * 0.01).Normalize());

	Color res;
	real dist = 0;
	renderer->rayTracing(ray, res, 0, 1, dist);
	// if (!(res != BLACK)){
	//	printf("%d %d\n",i,j);
		ray.prt();
		printf("dist = %lf\n", dist);
		ray(dist).prt();
		printf("res = ");
		res.prt();
	// }
// }

}

int main(int argc, char** argv)
{
	//set stack limit
	rlimit R;
	getrlimit(RLIMIT_STACK, &R);
	R.rlim_cur = R.rlim_max;
	setrlimit(RLIMIT_STACK, &R);

	Args args;
	args.parse(argc, argv);
	if (args.showHelp)
		return 0;

	Film* film = new Image(512, 512);
	film->setName("test");
	Camera* camera = new PerspectiveCamera(Vec3f(0, 0, -0), Vec3f(1, 0, 0), Vec3f(0, 0, 1), 60, 0, 10);
	//Camera *camera = new ProjectiveCamera(Vector(0, 5, 10), Vector(0, 0, -1), Vector(0, 1, 0), 90); 
	camera->setFilm(film);

	progressMessage("Camera constructed");

	Scene* scene = new Scene();
	int useScene = args.useScene;
	int debug = args.debug;

	//filmX 222, filmY 222
	Ray debugRay = Ray(Vec3f(0.000000, 0.000000, 1.000000), Vec3f(0.988487, 0.104990, 0.108951));

	camera->setPos(Vec3f(0, 0, 3));

	if (useScene == 0){
		//scene->loadObj();
	}

	//Material(Color aColor, real aRefl, real aRefr, real aDiff, real aSpec, real aRIndex = 1, Color Ka = BLACK)
	// Material* mirror = new Material;
	// mirror->addBxDF(new SpecularReflection(WHITE));

	Material* glass = new Material;
	glass->getBSDF()->addBxDF(new SpecularTransmission(WHITE));

	Material* matte = new Material;
	matte->getBSDF()->addBxDF(new LambertianReflection(PINK));

	// Image* lena = new Image("texture/lena.jpg");
	Material* wall = new Material;
	wall->getBSDF()->addBxDF(new LambertianReflection(CYAN));
	// wall->setTexture(lena);

	// Image* parquet = new Image("texture/parquet.jpg");
	Material* floor = new Material;
	floor->getBSDF()->addBxDF(new LambertianReflection(YELLOW));
	// floor->setTexture(parquet);

	Material* ceil = new Material;
	ceil->getBSDF()->addBxDF(new LambertianReflection(SKYBLUE));

	if (useScene == 666){
		DecimationTask(args);
		return 0;
	}

	//--------------------------------------test_scene 1----------------------------
	if (useScene == 1){
		// for (int i=0;i<5;++i){
		// 	for (int j=0;j<5;++j){
		// 		Primitive* obj = new Sphere(Vec3f(4,i-2,j-1),0.2);
		// 		obj->setMaterial(mat1);
		// 		scene->addObject(obj);
		// 	}
		// }

		Primitive* sphere1 = new Sphere(Vec3f(10, 0, 1), 1);
		sphere1->setMaterial(matte);
		scene->addObject(sphere1);

		// Primitive* obj2 = new Sphere(Vec3f(3, 0, 1), 1);
		// obj2->setMaterial(mirror);
		// //scene->addObject(obj2);

		Primitive* floor1 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 3);
		floor1->setMaterial(floor);
		scene->addObject(floor1);

		Primitive* ceil1 = new Plane(Vec3f(0, 0, -1), Vec3f(1, 0, 0), 5);
		ceil1->setMaterial(ceil);
		scene->addObject(ceil1);

		Primitive* wall1 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 12);
		wall1->setMaterial(wall);
		scene->addObject(wall1);

		// Primitive* obj4 = new TriangleMesh("test_data/cube.obj",
		// 	mirror, Matrix44::translation(testTransForCube));
		
		// Primitive* obj8 = new Box(testTransForCube, testTransForCube + Vec3f(1, 1, 1));
		// obj8->setMaterial(mirror);

		Light* plight1 = new Light(WHITE, 80, Vec3f(3, 0, 5));
		// scene->addLight(plight1);

		Light* alight1 = new AreaLight(WHITE, 80, Vec3f(5, 0, 4.99), Vec3f(0, 0, -1), Vec3f(1, 0, 0), 10);
		scene->addLight(alight1);
		// Light* light2 = new Light(WHITE, 80, Vec3f(0, 0, 3));
		// scene->addLight(light2);
		//Light* light3 = new Light(WHITE, 80, Vec3f(2.5, 0, 1.5));
		//scene->addLight(light3);
		
		//scene->addObject(obj4);

	}


	//--------------------------------------test_scene 2----------------------------
	// if (useScene == 2){
	// 	(Matrix44::scale(5) * Matrix44::rotateX(PI / 2) * Matrix44::rotateY(-PI / 2)) .prt();
	// 	Primitive* obj0 = new TriangleMesh("test_data/fixed.perfect.dragon.100K.0.07.obj",
	// 		mat2, Matrix44::scale(5) * Matrix44::rotateX(PI / 2) * Matrix44::rotateY(-PI / 2));
	// 	scene->addObject(obj0);

	// 	Light* light0 = new Light(WHITE * 10000 , Vec3f(0, 0, 4));
	// 	scene->addLight(light0);

	// 	Light* light1 = new Light(PINK / 2 , Vec3f(-4, 0, 0));
	// 	//scene->addLight(light1);
	// 	Light* light2 = new Light(WHITE / 2, Vec3f(0, 0, 4) );
	// 	//scene->addLight(light2);
	// 	Light* light3 = new Light(WHITE / 2, Vec3f(-3, 0, 0) );
	// 	//scene->addLight(light3);
	// 	Light* light4 = new Light(BLUE / 2, Vec3f(0, 0, 3));
	// 	//scene->addLight(light4);

	// 	Material* mat5 = new Material(RED, 0, 0., 0.8, 0, 1.0, WHITE/5);
	// 	Material* mat6 = new Material(GREEN, 0, 0., 0.8, 0, 1.0, WHITE/5);
	// 	Material* mat7 = new Material(WHITE, 0, 0., 0.8, 0, 1.0, WHITE/5);

	// 	// Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 6);
	// 	Primitive* obj5_1 = new Triangle(new Vertex(10, -10, -10), new Vertex(10, -10, 15), new Vertex(10, 10, -10));
	// 	obj5_1->setMaterial(mat7);
	// 	scene->addObject(obj5_1);

	// 	Primitive* obj5_2 = new Triangle(new Vertex(10, 10, 15), new Vertex(10, 10, -10), new Vertex(10, -10, 15));
	// 	obj5_2->setMaterial(mat7);
	// 	scene->addObject(obj5_2);

	// 	Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 6);
	// 	obj6->setMaterial(mat5);
	// 	//scene->addObject(obj6);

	// 	// Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 6);
	// 	// obj7->setMaterial(mat7);
	// 	// scene->addObject(obj7);
	// 	Primitive* obj7_1 = new Triangle(new Vertex(-2, -10, -5), new Vertex(10, -10, -5), new Vertex(-2, 10, -5));
	// 	obj7_1->setMaterial(mat7);
	// 	scene->addObject(obj7_1);

	// 	Primitive* obj7_2 = new Triangle(new Vertex(10, 10, -5), new Vertex(-2, 10, -5), new Vertex(10, -10, -5));
	// 	obj7_2->setMaterial(mat7);
	// 	scene->addObject(obj7_2);


	// 	Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 6);
	// 	obj8->setMaterial(mat6);
	// 	//scene->addObject(obj8);

	// 	Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 6);
	// 	obj9->setMaterial(mat7);
	// 	//scene->addObject(obj9);

	// 	camera->setPos(Vec3f(-10, 0, 0));
	// }

	// if (useScene == 3){
	// 	// Primitive* dragon = new TriangleMesh("test_data/fixed.perfect.dragon.100K.0.07.obj",
	// 	// 	mat2, Matrix44::scale(20));
	// 	// scene->addObject(dragon);
	// 	Primitive* horse = new TriangleMesh("test_data/horse.fine.90k.obj",
	// 		mat2, Matrix44::scale(20));
	// 	scene->addObject(horse);
		
	// 	// Primitive* obj0 = new TriangleMesh("test_data/block.obj", mat2);		
	// 	//scene->addObject(obj0);

	// 	camera->setPos(Vec3f(-40, 0, 0));

	// 	Light* light0 = new Light(WHITE , Vec3f(0, 0, 25));
	// 	scene->addLight(light0);

	// 	Light* light1 = new Light(WHITE / 3 , Vec3f(-20, 0, 0));
	// 	//scene->addLight(light1);
	// 	Light* light2 = new Light(WHITE / 3, Vec3f(0, 0, 20) );
	// 	//scene->addLight(light2);
	// 	Light* light3 = new Light(PINK / 2, Vec3f(-15, 0, 0) );
	// 	//scene->addLight(light3);
	// 	Light* light4 = new Light(YELLOW / 2, Vec3f(0, 0, 15));
	// 	//scene->addLight(light4);

	// 	Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 30);
	// 	obj5->setMaterial(mWhite);
	// 	scene->addObject(obj5);

	// 	Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 30);
	// 	obj6->setMaterial(mWhite);
	// 	scene->addObject(obj6);

	// 	Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 20);
	// 	obj7->setMaterial(mWhite);
	// 	scene->addObject(obj7);
	
	// 	Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 30);
	// 	obj8->setMaterial(mWhite);
	// 	scene->addObject(obj8);

	// 	Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 30);
	// 	obj9->setMaterial(mWhite);
	// 	scene->addObject(obj9);

	// 	camera->setPos(Vec3f(-25,0,0) );
	// }

	// //--------------------------------------test_scene 3----------------------------
	// if (useScene == 4){
	// 	camera->setPos(Vec3f(0, 0 ,-1));

	// 	Material* mat5 = new Material(RED, 0.3, 0., 0.5, 0, 1.0, WHITE/5);
	// 	Material* mat6 = new Material(GREEN, 0.3, 0., 0.5, 0, 1.0, WHITE/5);
	// 	Material* mat7 = new Material(WHITE, 0, 1, 0, 1, 1.33);
	// 	Material* mat8 = new Material(WHITE, 1, 0, 0, 1, 1);
	// 	Material* mat9 = new Material(WHITE, 0, 0., 0.5, 0, 1.0, WHITE/5);

	// 	Primitive* obj1 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 5);
	// 	obj1->setMaterial(mat9);

	// 	Primitive* obj2 = new Plane(Vec3f(0, 0, -1), Vec3f(1, 0, 0), 5);
	// 	obj2->setMaterial(mat9);

	// 	Primitive* obj3 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 5);
	// 	obj3->setMaterial(mat5);

	// 	Primitive* obj4 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 5);
	// 	obj4->setMaterial(mat6);

	// 	Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 10);
	// 	obj5->setMaterial(mat9);

	// 	Primitive* obj6 = new Sphere(Vec3f(6, -1, -4), 1);
	// 	obj6->setMaterial(mat7);

	// 	Primitive* obj7 = new Sphere(Vec3f(6, 1.5, -4), 1);
	// 	obj7->setMaterial(mat8);

	// 	scene->addObject(obj1);
	// 	scene->addObject(obj2);
	// 	scene->addObject(obj3);
	// 	scene->addObject(obj4);
	// 	scene->addObject(obj5);
	// 	scene->addObject(obj6);
	// 	scene->addObject(obj7);

	// 	//obj1->setMaterial(floor3);
	// 	//obj2->setMaterial(floor3);
	// 	//obj5->setMaterial(wall1);
	// 	//obj2->setMaterial(floor3);

	// 	//Light* light1 = new Light(SKYBLUE, Vec3f(5,-9,9));
	// 	//scene->addLight(light1);

	// 	//Light* light2 = new Light(YELLOW, Vec3f(5,9,-9));
	// 	//scene->addLight(light2);

	// 	Light* light3 = new Light(WHITE * 5000, Vec3f(5,0,4));
	// 	scene->addLight(light3);

	// 	Light* light4 = new AreaLight(WHITE, Vec3f(10, 0, 9.9), Vec3f(0, 0, -1), Vec3f(1, 0, 0));
	// 	//scene->addLight(light4);
	// }


	// if (useScene == 6){ // test speed of diff-split kdtree
	// 	// render [block.obj] takes about 40s (pc without charge)
	// 	// Material* mat1 = new Material(CYAN, 0, 0, 0.6, 0.4, 1, WHITE / 5);
	// 	// Material* mat2 = new Material(WHITE, 0, 1, 0.5, 0.1, 1.5, WHITE / 5);
	// 	// Material* mat4 = new Material(WHITE, 0.5, 0., 0.5, 0.5, 1.0, WHITE / 5);
	// 	Primitive* obj0 = new TriangleMesh("test_data/block.obj",
	// 		mat2);
	// 	scene->addObject(obj0);

	// 	camera->setPos(Vec3f(-40, 0, 0));
	// 	Light* light1 = new Light(WHITE / 3 , Vec3f(-20, 0, 0));
	// 	scene->addLight(light1);
	// 	Light* light2 = new Light(WHITE / 3, Vec3f(0, 0, 20) );
	// 	scene->addLight(light2);
	// 	Light* light3 = new Light(PINK / 2, Vec3f(-15, 0, 0) );
	// 	scene->addLight(light3);
	// 	Light* light4 = new Light(YELLOW / 2, Vec3f(0, 0, 15));
	// 	scene->addLight(light4);

	// 	Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 30);
	// 	obj5->setMaterial(mat4);
	// 	scene->addObject(obj5);

	// 	Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 30);
	// 	obj6->setMaterial(mat4);
	// 	scene->addObject(obj6);

	// 	Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 30);
	// 	obj7->setMaterial(mat4);
	// 	scene->addObject(obj7);
	
	// 	Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 30);
	// 	obj8->setMaterial(mat4);
	// 	scene->addObject(obj8);

	// 	Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 30);
	// 	obj9->setMaterial(mat4);
	// 	scene->addObject(obj9);

	// 	camera->setPos(Vec3f(-40,0,0) );
	// }
	
	scene->construct(debug);
	progressMessage("Scene constructed");

	Renderer* renderer = new TestRenderer();
	if (args.usePhoton)
		renderer = new PhotonRenderer();
	renderer->setScene(scene);
	renderer->setCamera(camera);

//for (int i = -5; i <= 0 ; ++ i){
//	renderer->getCamera()->setPos(Vec3f(i,0,0));
	progressMessage("Rendering");
	if (debug){
		emitDebugRay(renderer, debugRay);
	}else
		renderer->render(args);

	progressMessage("Done");

	if (!debug)
		renderer->show(true);
//}
	return 0;
}
