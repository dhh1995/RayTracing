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

using namespace Raytracer;

int main()
{
	Film* film = new Image(500, 500);
	film->setName("test");
	Camera* camera = new ProjectiveCamera(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 0, 1), 90);
	//Camera *camera = new ProjectiveCamera(Vector(0, 5, 10), Vector(0, 0, -1), Vector(0, 1, 0), 90); 
	camera->setFilm(film);

	puts("Camera constructed");

	Scene* scene = new Scene(WHITE / 5);
	int useScene = 3;
	int debug = 0;

	if (useScene == 0){
		scene->loadObj();
	}

	//Material(Color aColor, real aRefl, real aRefr, real aDiff, real aSpec, real aRIndex = 1, Color Ka = BLACK)
	Material* matPINK = new Material(PINK, 1, 0, 0.3, 0.6, 2, WHITE / 5);
	Material* matCYAN = new Material(CYAN, 1, 0, 0.5, 0.4, 1.5, WHITE / 5);
	Material* mat1 = new Material(CYAN, 0, 0, 0.6, 0.4, 1, WHITE / 5);
	Material* mat2 = new Material(YELLOW, 0, 1, 0.5, 0.1, 1.5, WHITE / 5);
	
	Material* wall1 = new Material(WHITE, 0.1, 0. , 0.8, 0.2, 1.0, WHITE / 10);
	Image* image1 = new Image("texture/lena.jpg");
	Texture* texture1 = new Texture(image1, 10, 10);
	wall1->setTexture(texture1);
	Material* floor3 = new Material(WHITE, 0.1, 0. , 0.8, 0.2, 1.0, WHITE / 10);
	Image* image3 = new Image("texture/parquet.jpg");
	Texture* texture3 = new Texture(image3, 50, 50);
	floor3->setTexture(texture3);

	Material* mat4 = new Material(WHITE, 0.3, 0., 0.5, 0.5, 1.0, WHITE/5);

	//--------------------------------------test_scene 1----------------------------
	if (useScene == 1){

		// for (int i=0;i<5;++i){
		// 	for (int j=0;j<5;++j){
		// 		Primitive* obj = new Sphere(Vec3f(4,i-2,j-1),0.2);
		// 		obj->setMaterial(mat1);
		// 		scene->addObject(obj);
		// 	}
		// }

		Primitive* obj1 = new Sphere(Vec3f(20, 0, 1), 2);
		obj1->setMaterial(mat1);
		scene->addObject(obj1);

		Primitive* obj2 = new Sphere(Vec3f(3, 1, 0), 0.5);
		obj2->setMaterial(mat2);
		//scene->addObject(obj2);

		Primitive* obj3 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 1);
		obj3->setMaterial(floor3);
		scene->addObject(obj3);

		Primitive* obj7 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, 0, -1), 20);
		obj7->setMaterial(wall1);
		scene->addObject(obj7);

		// Primitive* obj4 = new TriangleMesh("test_data/cube.obj",
		// 	mat4, Vec3f(2, 0.5, -1));
		Primitive* obj4 = new TriangleMesh("test_data/cube.obj",
			mat2, Vec3f(2, -0.5, 0));
		scene->addObject(obj4);

		Primitive* obj5 = new Sphere(Vec3f(0, 0, 0), 1000);
		obj5->setMaterial(mat2);
		//scene->addObject(obj5);

		Light* light1 = new Light(WHITE/2, Vec3f(3, 0, 5));
		scene->addLight(light1);
		Light* light2 = new Light(WHITE/2, Vec3f(0, 0, 3));
		scene->addLight(light2);
		
		camera->setPos(Vec3f(0,0,1));
	}


	//--------------------------------------test_scene 2----------------------------
	if (useScene == 2){
		Primitive* obj0 = new TriangleMesh("test_data/dinosaur.2k.obj",
			mat2, Vec3f(0, 0, 0));
		scene->addObject(obj0);

		camera->setPos(Vec3f(-50, 0, 0));
		Light* light1 = new Light(GREEN, Vec3f(-40, 0, 0));
		scene->addLight(light1);
		Light* light2 = new Light(WHITE / 3, Vec3f(0, 0, 40) );
		scene->addLight(light2);
		Light* light3 = new Light(WHITE / 3, Vec3f(-30, 0, 0) );
		scene->addLight(light3);
		Light* light4 = new Light(RED, Vec3f(0, 0, 30));
		scene->addLight(light4);

		Primitive* obj5 = new Plane(Vec3f(-1, 0, 0), Vec3f(0, -1, 0), 50);
		obj5->setMaterial(mat4);
		scene->addObject(obj5);

		Primitive* obj6 = new Plane(Vec3f(0, 1, 0), Vec3f(-1, 0, 0), 50);
		obj6->setMaterial(mat4);
		scene->addObject(obj6);

		Primitive* obj7 = new Plane(Vec3f(0, 0, 1), Vec3f(1, 0, 0), 50);
		obj7->setMaterial(mat4);
		scene->addObject(obj7);
	
		Primitive* obj8 = new Plane(Vec3f(0, -1, 0), Vec3f(1, 0, 0), 50);
		obj8->setMaterial(mat4);
		scene->addObject(obj8);

		Primitive* obj9 = new Plane(Vec3f(0, 0, -1), Vec3f(-1, 0, 0), 50);
		obj9->setMaterial(mat4);
		scene->addObject(obj9);
	}

	//--------------------------------------test_scene 3----------------------------
	if (useScene == 3){
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

	// Pri *obj1 = new Sphere(Vector(-6, 0, -10), 5);
	// Pri *obj2 = new Sphere(Vector(6, 0, -10), 5);
	// Pri *obj3 = new Plane(Vector(0, -5, 0), Vector(0, 1, 0));
	// Pri *obj4 = new Plane(Vector(-20, 0, 0), Vector(1, 0, 0));
	// Pri *obj5 = new Plane(Vector(20, 0, 0), Vector(-1, 0, 0));
	// Pri *obj6 = new Plane(Vector(0, 0, -20), Vector(0, 0, 1));
	// Pri *obj7 = new Plane(Vector(0, 20, 0), Vector(0, -1, 0));

	// obj1->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene);
	// obj2->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene);
	// obj3->set_material(new Phong(REFL_DIFF, Vector(.75,.75,.75),      Vector::Zero))->add_to_scene(scene);
	// obj4->set_material(new Phong(REFL_DIFF, Vector(.75,.25,.25),      Vector::Zero))->add_to_scene(scene);
	// obj5->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.75),      Vector::Zero))->add_to_scene(scene);
	// obj6->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.25),      Vector::Zero))->add_to_scene(scene);
	// obj7->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.25),      Vector::Zero))->add_to_scene(scene);
	// lite->set_material(new Phong(REFL_DIFF, Vector::Zero, Vector(100, 100, 100)))->add_to_scene(scene);

	scene->construct();
	puts("Scene constructed");

	Renderer* renderer = new TestRenderer();
	renderer->setScene(scene);
	renderer->setCamera(camera);

//for (int i = -5; i <= 0 ; ++ i){
//	renderer->getCamera()->setPos(Vec3f(i,0,0));
	puts("Rendering");
	if (debug){
		Ray debugRay = Ray(Vec3f(2.296522, 0.046868, -0.046868), Vec3f(0.999584, 0.020400, -0.020400));
		Color res;
		real dist;
		renderer->rayTracing(debugRay, res, 0, 1, dist);
		printf("dist = %lf\n",dist);
	}else
		renderer->render();

	puts("Done");

	if (!debug)
		renderer->show();
//}
	return 0;
}
