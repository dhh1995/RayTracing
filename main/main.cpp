#include <sys/resource.h>

#include "core/common.h"
//#include "obj_parser/SimpleObject.h"
#include "core/args.h"
#include "core/camera.h"
#include "core/scene.h"
#include "core/light.h"
#include "renderers/testrenderer.h"
#include "renderers/photonrenderer.h"
#include "film/image.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/trianglemesh.h"
#include "decimation/decimation.h"

using namespace Raytracer;

void emitDebugRay(Renderer* renderer, Ray ray){
	Color res;
	real dist = 0;
	renderer->rayTracing(ray, res, 0);
	ray.prt();
	printf("dist = %lf\n", dist);
	ray(dist).prt();
	printf("res = ");
	res.prt();
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

	Film* film = new Image(500, 500);
	film->setName("test");
	Camera* camera = new PerspectiveCamera(Vec3f(0, 0, -0), Vec3f(1, 0, 0), Vec3f(0, 0, 1),
											args.fov, args.lensRadius, args.focalDist);
	camera->setFilm(film);

	progressMessage("Camera constructed");

	Scene* scene = new Scene(WHITE / 5);
	int useScene = args.useScene;
	int debug = args.debug;
	int useBox = 2;

	//filmX 222, filmY 222
	Ray debugRay = Ray(Vec3f(0.000000, 0.000000, 1.000000), Vec3f(0.988487, 0.104990, 0.108951));

	camera->setPos(Vec3f(0, 0, 3));

	//Material(Ka, Kd, Ks, fresnel)
	Color Background = WHITE / 5;
	
	Material* mPink		= new Material(Background, PINK, 	BLACK);
	Material* mGreen	= new Material(Background, AGREEN,	BLACK);
	Material* mBlue		= new Material(Background, ABLUE,	BLACK);
	Material* mRed		= new Material(Background, ARED,	BLACK);
	Material* mYellow	= new Material(Background, YELLOW,	BLACK);
	Material* mWhite	= new Material(Background, WHITE,	BLACK);

	Material* mat1 		= new Material(Background, CYAN * 0.6, CYAN * 0.4);
	Material* mat2 		= new Material(Background, PINK,	PINK, new Fresnel(1.5));
	
	Material* wall1		= new Material(Background, WHITE * 0.8, BLACK);
	Image* image0 = new Image("texture/lena.jpg");
	Image* image1 = new Image("texture/world1.jpg");
	Texture* texture1 = new Texture(image0, 20, 20);
	wall1->setTexture(texture1);
	Material* floor3	= new Material(Background, WHITE * 0.8, BLACK);
	Image* image3 = new Image("texture/parquet.jpg");
	Texture* texture3 = new Texture(image3, 50, 50);
	floor3->setTexture(texture3);

	Material* mat4		= new Material(Background, WHITE * 0.8, BLACK);

	if (useScene == 666){ //Decimation Task
		Decimation::Run(args);
		return 0;
	}

	if (useScene == -1){
		if (!scene->loadObj("model", args.obj)){
			colorMessage("load scene unsuccessfully", 1);
			return 0;
		}
	}

	if (useScene == 0){
		if (!scene->loadObj("model", args.obj)){
			colorMessage("load scene unsuccessfully", 1);
			return 0;
		}
		int power = 32;
		if (args.usePhoton)
			power = 100;
		Light* light4 = new AreaLight(WHITE, Vec3f(278, 548.8, 279.5), power, Vec3f(0, -1, 0), Vec3f(1, 0, 0), 1, 200, 200);
		scene->addLight(light4);

		camera->setPos(Vec3f(278, 273, -400));
		camera->setLookAt(Vec3f(0, 0, 1), Vec3f(0, 1, 0));
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

		Vec3f testTransForCube(3, -0.5, 1.5);

		Primitive* obj1 = new Sphere(Vec3f(7, 0, 1), 0.2);
		obj1->setMaterial(mat1);
		//scene->addObject(obj1);

		Primitive* obj2 = new Sphere(Vec3f(3, 0, 1), 1);
		obj2->setMaterial(mat2);
		//scene->addObject(obj2);

		Primitive* obj3 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 0);
		obj3->setMaterial(floor3);
		scene->addObject(obj3);

		Primitive* obj7 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 8);
		obj7->setMaterial(wall1);
		scene->addObject(obj7);

		Primitive* obj7_2 = new Plane(Vec3f(0, 1, 0), Vec3f(0, 0, -1), 8);
		obj7_2->setMaterial(mat1);
		scene->addObject(obj7_2);

		// Primitive* obj4 = new TriangleMesh("test_data/cube.obj",
		// 	mat4, Vec3f(2, 0.5, -1));
		Primitive* obj4 = new TriangleMesh("test_data/cube.obj",
			mat2, Matrix44::translation(testTransForCube));
		//scene->addObject(obj4);
		
		Primitive* obj9 = new Sphere(Vec3f(3, 0, 1), 1);
		obj9->setMaterial(mat2);
		//scene->addObject(obj9);

		Primitive* obj8 = new Box(testTransForCube, testTransForCube + Vec3f(1, 1, 1));
		obj8->setMaterial(mat2);
		//scene->addObject(obj8);

		Primitive* obj5 = new Sphere(Vec3f(0, 0, 0), 1000);
		obj5->setMaterial(mat2);
		//scene->addObject(obj5);

		Light* light1 = new AreaLight(WHITE, Vec3f(3, 0, 6), 30, Vec3f(0, 0, -1), Vec3f(1, 0, 0), 1, 2, 2);
		scene->addLight(light1);
		// Light* light2 = new AreaLight(WHITE, Vec3f(0, 0, 4), 100);
		// scene->addLight(light2);
		//Light* light3 = new Light(WHITE, Vec3f(2.5, 0, 1.5));
		//scene->addLight(light3);
		
		if (useBox >> 1 & 1)
			scene->addObject(obj8);
		if (useBox & 1)
			scene->addObject(obj4);

		camera->setPos(Vec3f(-3, 0, 4));
	}

	//--------------------------------------test_scene 2----------------------------
	if (useScene == 2){
		Material* mat1 = new Material(Background, CYAN * 0.6, CYAN * 0.4);
		Material* mat2 = new Material(Background, PINK * 0.6, PINK * 0.4);
	
		// (Matrix44::scale(5) * Matrix44::rotateX(PI / 2) * Matrix44::rotateY(-PI / 2)) .prt();
		int power = 1500;
		if (args.usePhoton)
			power = 10000;
		for (int i = 0; i < 8; ++ i){
			Primitive* dragon = new TriangleMesh("test_data/fixed.perfect.dragon.100K.0.07.obj",
				i & 1 ? mat1 : mat2,
				Matrix44::translation(Vec3f(4 * i, 5 * i, 0)) *
				Matrix44::scale(5) * Matrix44::rotateX(PI / 2) * Matrix44::rotateY(-PI / 2));
			scene->addObject(dragon);
			Light* light = new Light(WHITE, Vec3f(i * 5, i * 5, 5), power);
			scene->addLight(light);
		}
		// Light* largeLight = new AreaLight(WHITE, Vec3f(-11, 5, 0), power, Vec3f(1, 0, 0), Vec3f(0, 1, 0), 2, 15, 15);
		// scene->addLight(largeLight);

		// Material* mat5 = new Material(Background, WHITE * 0.8, BLACK);
		// Material* mat6 = new Material(Background, WHITE * 0.8, BLACK);
		Material* mat7 = new Material(Background, LIGHTGREEN, BLACK);

		// Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 6);
		Primitive* obj5_1 = new Triangle(new Vertex(48, -35, -10), new Vertex(48, -35, 60), new Vertex(48, 80, -10));
		obj5_1->setMaterial(mat7);
		scene->addObject(obj5_1);

		Primitive* obj5_2 = new Triangle(new Vertex(48, 80, 60), new Vertex(48, 80, -10), new Vertex(48, -35, 60));
		obj5_2->setMaterial(mat7);
		scene->addObject(obj5_2);

		Primitive* obj7_1 = new Triangle(new Vertex(-50, -10, -5), new Vertex(50, -10, -5), new Vertex(-50, 50, -5));
		obj7_1->setMaterial(mat7);
		scene->addObject(obj7_1);

		Primitive* obj7_2 = new Triangle(new Vertex(50, 50, -5), new Vertex(-50, 50, -5), new Vertex(50, -10, -5));
		obj7_2->setMaterial(mat7);
		scene->addObject(obj7_2);

		camera->setPos(Vec3f(-25, 15, 0));
	}

	if (useScene == 3){
		// Primitive* dragon = new TriangleMesh("test_data/fixed.perfect.dragon.100K.0.07.obj",
		// 	mat2, Matrix44::scale(20));
		// scene->addObject(dragon);
		Primitive* horse = new TriangleMesh("test_data/horse.fine.90k.obj",
			mat2, Matrix44::scale(20));
		scene->addObject(horse);
		
		// Primitive* obj0 = new TriangleMesh("test_data/block.obj", mat2);		
		//scene->addObject(obj0);

		camera->setPos(Vec3f(-40, 0, 0));

		int power = 10000;
		if (args.usePhoton)
			power = 100000;
		Light* light0 = new Light(WHITE , Vec3f(0, 0, 25), power);
		scene->addLight(light0);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 30);
		obj5->setMaterial(mBlue);
		scene->addObject(obj5);

		Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 30);
		obj6->setMaterial(mGreen);
		scene->addObject(obj6);

		Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 20);
		obj7->setMaterial(mWhite);
		scene->addObject(obj7);
	
		Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 30);
		obj8->setMaterial(mRed);
		scene->addObject(obj8);

		Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 30);
		obj9->setMaterial(mWhite);
		scene->addObject(obj9);

		camera->setPos(Vec3f(-50,0,0) );
	}

	//--------------------------------------test_scene 3----------------------------
	if (useScene == 4){
		camera->setPos(Vec3f(-5, 0 ,-1));
		Material* mat5 = new Material(Background, ARED, BLACK);
		Material* mat6 = new Material(Background, AGREEN, BLACK);
		Material* mat7 = new Material(Background, BLACK, WHITE, new Fresnel(1.33));
		Material* mat8 = new Material(Background, BLACK, WHITE);
		Material* mat9 = new Material(Background, WHITE, BLACK);

		Primitive* obj1 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 5);
		obj1->setMaterial(mat9);

		Primitive* obj2 = new Plane(Vec3f(0, 0, -1), Vec3f(1, 0, 0), 5);
		obj2->setMaterial(mat9);

		Primitive* obj3 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 5);
		obj3->setMaterial(mat5);

		Primitive* obj4 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 5);
		obj4->setMaterial(mat6);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 10);
		obj5->setMaterial(mat9);

		Primitive* obj6 = new Sphere(Vec3f(6, -1, -4), 1);
		obj6->setMaterial(mat7);

		Primitive* obj7 = new Sphere(Vec3f(6, 1.5, -4), 1);
		obj7->setMaterial(mat8);

		scene->addObject(obj1);
		scene->addObject(obj2);
		scene->addObject(obj3);
		scene->addObject(obj4);
		scene->addObject(obj5);
		scene->addObject(obj6);
		scene->addObject(obj7);

		//obj1->setMaterial(floor3);
		//obj2->setMaterial(floor3);
		//obj5->setMaterial(wall1);
		//obj2->setMaterial(floor3);

		//Light* light1 = new Light(SKYBLUE, Vec3f(5,-9,9));
		//scene->addLight(light1);

		//Light* light2 = new Light(YELLOW, Vec3f(5,9,-9));
		//scene->addLight(light2);

		Light* light3 = new Light(WHITE, Vec3f(6,0,4), 500);
		// scene->addLight(light3);

		Light* light4 = new AreaLight(WHITE, Vec3f(7, 0, 4.9), 500, Vec3f(0, 0, -1), Vec3f(1, 0, 0), 2, 2, 2);
		scene->addLight(light4);
	}


	if (useScene == 6){ // test speed of diff-split kdtree
		// render [block.obj] takes about 40s (pc without charge)
		// Material* mat1 = new Material(CYAN, 0, 0, 0.6, 0.4, 1, WHITE / 5);
		// Material* mat2 = new Material(WHITE, 0, 1, 0.5, 0.1, 1.5, WHITE / 5);
		// Material* mat4 = new Material(WHITE, 0.5, 0., 0.5, 0.5, 1.0, WHITE / 5);
		Primitive* obj0 = new TriangleMesh("test_data/block.obj",
			mat2);
		scene->addObject(obj0);

		camera->setPos(Vec3f(-40, 0, 0));
		Light* light1 = new Light(WHITE / 3 , Vec3f(-20, 0, 0));
		scene->addLight(light1);
		Light* light2 = new Light(WHITE / 3, Vec3f(0, 0, 20) );
		scene->addLight(light2);
		Light* light3 = new Light(PINK / 2, Vec3f(-15, 0, 0) );
		scene->addLight(light3);
		Light* light4 = new Light(YELLOW / 2, Vec3f(0, 0, 15));
		scene->addLight(light4);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 30);
		obj5->setMaterial(mat4);
		scene->addObject(obj5);

		Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 30);
		obj6->setMaterial(mat4);
		scene->addObject(obj6);

		Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 30);
		obj7->setMaterial(mat4);
		scene->addObject(obj7);
	
		Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 30);
		obj8->setMaterial(mat4);
		scene->addObject(obj8);

		Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 30);
		obj9->setMaterial(mat4);
		scene->addObject(obj9);

		camera->setPos(Vec3f(-40,0,0) );
	}
	
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
