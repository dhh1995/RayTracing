#include "core/common.h"
//#include "obj_parser/SimpleObject.h"
#include "core/camera.h"
#include "core/scene.h"
#include "core/light.h"
#include "renderers/testrenderer.h"
#include "film/image.h"
#include "shapes/sphere.h"
#include "shapes/plane.h"
#include "shapes/trianglemesh.h"
#include "decimation/demesh.h"

using namespace Raytracer;
using namespace Decimation;

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

int main()
{
	Film* film = new Image(500, 500);
	film->setName("test");
	Camera* camera = new ProjectiveCamera(Vec3f(0, 0.001, -0.001), Vec3f(1, 0, 0), Vec3f(0, 0, 1), 90);
	//Camera *camera = new ProjectiveCamera(Vector(0, 5, 10), Vector(0, 0, -1), Vector(0, 1, 0), 90); 
	camera->setFilm(film);

	progressMessage("Camera constructed");

	Scene* scene = new Scene(WHITE / 5);
	int useScene = 666;
	int debug = 0;
	int useBox = 1;


	//filmX 222, filmY 222
	Ray debugRay = Ray(Vec3f(0.000000, 0.000000, 1.000000), Vec3f(0.988487, 0.104990, 0.108951));
	//filmX 220, filmY 205
	// debugRay.setDir(Vec3f(0.977862, 0.174408, 0.115619));
	// debugRay.setDir(Vec3f(1, 0, 0.2).Normalize());
	//filmX 218, filmY 250
	// debugRay.setDir(Vec3f(0.992107, -0.005965, 0.125256));
	// debugRay = Ray(Vec3f(-20.000000, 0.000000, 0.000000), Vec3f(1, -0.1, 0.1).Normalize());

	camera->setPos(Vec3f(0, 0, 3));

	if (useScene == 0){
		scene->loadObj();
	}

	Vec3f testTransForCube(2, -1.5, 0.5);

	//Material(Color aColor, real aRefl, real aRefr, real aDiff, real aSpec, real aRIndex = 1, Color Ka = BLACK)
	Material* matPINK = new Material(PINK, 1, 0, 0.3, 0.6, 2, WHITE / 5);
	Material* matCYAN = new Material(CYAN, 1, 0, 0.5, 0.4, 1.5, WHITE / 5);
	Material* mat1 = new Material(CYAN, 0, 0, 0.6, 0.4, 1, WHITE / 5);
	Material* mat2 = new Material(WHITE, 0, 1, 0.5, 0.1, 1.5, WHITE / 5);
	
	Material* wall1 = new Material(WHITE, 0., 0. , 0.8, 0.2, 1.0, WHITE / 10);
	Image* image0 = new Image("texture/lena.jpg");
	Image* image1 = new Image("texture/world1.jpg");
	Texture* texture1 = new Texture(image0, 20, 20);
	wall1->setTexture(texture1);
	Material* floor3 = new Material(WHITE, 0., 0. , 0.8, 0.2, 1.0, WHITE / 10);
	Image* image3 = new Image("texture/parquet.jpg");
	Texture* texture3 = new Texture(image3, 50, 50);
	floor3->setTexture(texture3);

	Material* mat4 = new Material(WHITE, 0, 0., 0.5, 0.5, 1.0, WHITE/5);

	if (useScene == 666){
		enum OBJ{
			DRAGON,
			BUNNY,
			DINASAUR,
			CUBE,
		};
		int testModel = DRAGON;
		progressMessage("start loading");
		switch(testModel){
		case DRAGON:{
			DeMesh* dragon = new DeMesh("test_data/fixed.perfect.dragon.100K.0.07.obj", mat1);
			progressMessage("start decimation");
			dragon->decimation(0.01, dragon->diagonalLength() / 100.0);
			dragon->dump("dragon_res.obj");
		}
			break;
		case BUNNY:{
			DeMesh* bunny = new DeMesh("test_data/bunny.fine.obj", mat1);
			progressMessage("start decimation");
			bunny->decimation(0.1);
			bunny->dump("bunny_res.obj");
		}
			break;
		case DINASAUR:{
			DeMesh* dinosaur = new DeMesh("test_data/dinosaur.2k.obj", mat1);
			progressMessage("start decimation");
			dinosaur->decimation(0.3, 0 * dinosaur->diagonalLength() / 100.0);
			dinosaur->dump("dinosaur_res.obj");
		}
			break;
		case CUBE:{
			DeMesh* cube = new DeMesh("test_data/cube.obj", mat1);
			progressMessage("start decimation");
			cube->decimation(0.9);
			cube->dump("cube_res.obj");
		}
			break;
		}
		progressMessage("end dumping");
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


		Primitive* obj1 = new Sphere(Vec3f(7, 0, 1), 0.2);
		obj1->setMaterial(mat1);
		//scene->addObject(obj1);

		Primitive* obj2 = new Sphere(Vec3f(3, 0, 1), 1);
		obj2->setMaterial(mat2);
		//scene->addObject(obj2);

		Primitive* obj3 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 1);
		obj3->setMaterial(floor3);
		scene->addObject(obj3);

		Primitive* obj7 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 8);
		obj7->setMaterial(wall1);
		scene->addObject(obj7);

		// Primitive* obj7_2 = new Plane(Vec3f(0, -1, 0), Vec3f(0, 0, -1), 8);
		// obj7_2->setMaterial(mat1);
		// scene->addObject(obj7_2);

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

		Light* light1 = new Light(WHITE/2, Vec3f(3, 0, 5));
		scene->addLight(light1);
		Light* light2 = new Light(WHITE, Vec3f(0, 0, 3));
		scene->addLight(light2);
		//Light* light3 = new Light(WHITE, Vec3f(2.5, 0, 1.5));
		//scene->addLight(light3);
		
		if (useBox >> 1 & 1)
			scene->addObject(obj8);
		if (useBox & 1)
			scene->addObject(obj4);

	}


	//--------------------------------------test_scene 2----------------------------
	if (useScene == 2){
		Primitive* obj0 = new TriangleMesh("test_data/dinosaur.2k.obj",
			mat1, Matrix44::scale(0.05));
		scene->addObject(obj0);

		//Light* light0 = new Light(WHITE, Vec3f(-20, 0, 0));
		//scene->addLight(light0);

		Light* light1 = new Light(PINK / 2 , Vec3f(-4, 0, 0));
		scene->addLight(light1);
		Light* light2 = new Light(WHITE / 3, Vec3f(0, 0, 4) );
		scene->addLight(light2);
		Light* light3 = new Light(WHITE / 3, Vec3f(-3, 0, 0) );
		scene->addLight(light3);
		Light* light4 = new Light(BLUE / 2, Vec3f(0, 0, 3));
		//scene->addLight(light4);

		Material* newmat4 = new Material(WHITE, 0.5, 0., 0.5, 0.5, 1.0, WHITE/5);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 6);
		obj5->setMaterial(mat4);
		scene->addObject(obj5);

		Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 6);
		obj6->setMaterial(mat4);
		scene->addObject(obj6);

		Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 6);
		obj7->setMaterial(newmat4);
		scene->addObject(obj7);
	
		Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 6);
		obj8->setMaterial(mat4);
		scene->addObject(obj8);

		Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 6);
		obj9->setMaterial(newmat4);
		scene->addObject(obj9);

		camera->setPos(Vec3f(-4, 0, 0));

	}

	if (useScene == 3){
		Primitive* dragon = new TriangleMesh("test_data/fixed.perfect.dragon.100K.0.07.obj",
			mat1, Matrix44::scale(20));
		scene->addObject(dragon);
		Primitive* obj0 = new TriangleMesh("test_data/block.obj",
			mat2);
		//scene->addObject(obj0);

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

	//--------------------------------------test_scene 3----------------------------
	if (useScene == 4){
		camera->setPos(Vec3f(-5, 0 ,0));

		Material* mat5 = new Material(RED, 0.3, 0., 0.5, 0.5, 1.0, WHITE/5);
		Material* mat6 = new Material(GREEN, 0.3, 0., 0.5, 0.5, 1.0, WHITE/5);
		Material* mat7 = new Material(WHITE, 0, 1, 0, 0, 1.33);
		Material* mat8 = new Material(WHITE, 1, 0, 0.1, 0.1, 1);

		Primitive* obj1 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 10);
		obj1->setMaterial(mat4);

		Primitive* obj2 = new Plane(Vec3f(0, 0, -1), Vec3f(1, 0, 0), 10);
		obj2->setMaterial(mat4);

		Primitive* obj3 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 10);
		obj3->setMaterial(mat5);

		Primitive* obj4 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 10);
		obj4->setMaterial(mat6);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 20);
		obj5->setMaterial(mat4);

		Primitive* obj6 = new Sphere(Vec3f(6, -4, -6), 3);
		obj6->setMaterial(mat7);

		Primitive* obj7 = new Sphere(Vec3f(8, 3, -7), 3);
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

		Light* light3 = new Light(WHITE, Vec3f(10,0,9));
		scene->addLight(light3);

		Light* light4 = new AreaLight(WHITE, Vec3f(10, 0, 9.9), Vec3f(0, 0, -1), Vec3f(1, 0, 0));
		//scene->addLight(light4);
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
	renderer->setScene(scene);
	renderer->setCamera(camera);

//for (int i = -5; i <= 0 ; ++ i){
//	renderer->getCamera()->setPos(Vec3f(i,0,0));
	progressMessage("Rendering");
	if (debug){
		emitDebugRay(renderer, debugRay);
	}else
		renderer->render();

	progressMessage("Done");

	if (!debug)
		renderer->show();
//}
	return 0;
}
