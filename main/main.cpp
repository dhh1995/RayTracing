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
	Camera* camera = new ProjectiveCamera(Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0, 1, 0), 90);
	//Camera *camera = new ProjectiveCamera(Vector(0, 5, 10), Vector(0, 0, -1), Vector(0, 1, 0), 90); 
	camera->setFilm(film);

	puts("Camera constructed");

	Scene* scene = new Scene(WHITE / 10);

	//scene->loadObj();
	
	//--------------------------------------test_scene 1----------------------------
	
	Primitive* obj1 = new Sphere(Vec3f(4, 1, -2), 2);
	obj1->setMaterial(new Material(RED, 1, 0.6));
	scene->addObject(obj1);

	Primitive* obj2 = new Sphere(Vec3f(4, 1, 2), 2);
	obj2->setMaterial(new Material(BLUE, 1, 0.5));
	scene->addObject(obj2);

	// Primitive* obj3 = new Plane(Vec3f(0, 1, 0), 1);
	// Material* floor3 = new Material(WHITE, 0.5, 0.8, WHITE / 10);
	// Image* image3 = new Image("/home/dhh1995/RayTracing/texture/parquet.jpg");
	// Texture* texture3 = new Texture(image3);
	// floor3->setTexture(texture3);
	// obj3->setMaterial(floor3);
	// scene->addObject(obj3);

	// Primitive* obj4 = new Primitive();
	// obj4->setShape(new Triangle(Vec3f(1, 0, 0), Vec3f(1, 1, 2), Vec3f(1, 1, -2)));
	// //obj4->setShape(new Triangle(Vec3f(3, 0, 0), Vec3f(3, 1, 0), Vec3f(3, 1, 1)));
	// obj4->setMaterial(new Material(BLUE, 0, 0.5, WHITE / 10));
	// scene->addObject(obj4);

	// Primitive* obj5 = new TriangleMesh("/home/dhh1995/RayTracing/test_data/cube.obj", Vec3f(2, 0, 0));
	// //obj5->setShape(new Triangle(Vec3f(2,0,0), Vec3f(2,1,1), Vec3f(2,1,-1) ));
	// obj5->setMaterial(new Material(BLUE, 0.1, 0.5, WHITE));
	// scene->addObject(obj5);

	camera->setPos(Vec3f(-5, 0 ,0));

	Light* light1 = new Light(Vec3f(-8, 2, 0), GREEN);
	scene->addLight(light1);
	Light* light2 = new Light(Vec3f(0, 1, 0), WHITE);
	scene->addLight(light2);

	//--------------------------------------test_scene 2----------------------------

	// camera->setPos(Vec3f(-5, 0 ,0));
	// Primitive* obj1 = new Primitive();
	// obj1->setShape(new Plane(Vec3f(0, 0, 1), 10));
	// obj1->setMaterial(new Material(RED, 0.3, 0.6));
	// scene->addObject(obj1);

	// Primitive* obj2 = new Primitive();
	// obj2->setShape(new Plane(Vec3f(0, 0, -1), 10));
	// obj2->setMaterial(new Material(GREEN, 0, 0.5));
	// scene->addObject(obj2);

	// Primitive* obj3 = new Primitive();
	// obj3->setShape(new Plane(Vec3f(0, -1, 0), 10));
	// obj3->setMaterial(new Material(WHITE, 0, 0.5));
	// scene->addObject(obj3);

	// Primitive* obj4 = new Primitive();
	// obj4->setShape(new Plane(Vec3f(0, 1, 0), 10));
	// obj4->setMaterial(new Material(WHITE, 0, 0.5));
	// scene->addObject(obj4);

	// Primitive* obj5 = new Primitive();
	// obj5->setShape(new Plane(Vec3f(-1, 0, 0), 20));
	// obj5->setMaterial(new Material(WHITE, 0, 0.5));
	// scene->addObject(obj5);

	// Primitive* obj6 = new Primitive();
	// obj6->setShape(new Sphere(Vec3f(5, -5, 0), 2));
	// obj6->setMaterial(new Material(WHITE, 1, 0.5));
	// scene->addObject(obj6);

	// Primitive* obj7 = new Primitive();
	// obj7->setShape(new Sphere(Vec3f(5, 5, 0), 2));
	// obj7->setMaterial(new Material(WHITE, 1, 0.5));
	// scene->addObject(obj7);

	// Light* light1 = new Light(Vec3f(5,9,0), WHITE / 3);
	// scene->addLight(light1);

	// Light* light2 = new Light(Vec3f(5,-9,0), WHITE / 3);
	// scene->addLight(light2);

	// Light* light3 = new Light(Vec3f(5,0,0), WHITE / 3);
	// scene->addLight(light3);

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

	puts("Scene constructed");

	Renderer* renderer = new TestRenderer();
	renderer->setScene(scene);
	renderer->setCamera(camera);

//for (int i = -5; i <= 0 ; ++ i){
//	renderer->getCamera()->setPos(Vec3f(i,0,0));
	puts("Rendering");
	renderer->render();

	puts("Done");

	renderer->show();
//}
	return 0;
}
