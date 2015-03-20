#include "stdafx.h"
#include "Testcase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"

void configCamera(Camera* camera)
{
	camera->setIsOrthProjection(false);
	camera->setFarClippingPlane(1000);
	camera->setNearClippingPlane(0.3f);
	camera->setFieldOfView(60);

	//camera->setTranslation(0,0,0);
	//camera->setRotation(0,0,0);
}
std::shared_ptr<GameObject> loadGameObject()
{
	std::shared_ptr<GameObject> go(new GameObject());
	//auto vbuffer = go->getVertexBuffer(4);
	//auto ibuffer = go->getIndexBuffer(6);
	//vbuffer[0].position.x = ;
	//ibuffer[]
	return go;
}

std::unique_ptr<Scene> configScene()
{
	std::unique_ptr<Scene> scene(new Scene());

	configCamera(scene->mainCamera());

	auto obj = loadGameObject();

	scene->addGameObject(obj);

	scene->isDrawline = true;

	return scene;
}