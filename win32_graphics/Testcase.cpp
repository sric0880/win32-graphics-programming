#include "stdafx.h"
#include "Testcase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"

#include "OutputDebug.h"

void configCamera(Camera* camera)
{
	camera->setIsOrthProjection(false);
	camera->setFarClippingPlane(1000);
	camera->setNearClippingPlane(0.3f);
	camera->setFieldOfView(60);

	//camera->setTranslation(0,0,0);
	//camera->setRotation(0,0,0);
}
GameObject loadGameObject()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);

	vbuffer[0].position.x = -100;
	vbuffer[0].position.y = 100;
	vbuffer[0].position.z = 0;
	vbuffer[1].position.x = -100;
	vbuffer[1].position.y = -100;
	vbuffer[1].position.z = 0;
	vbuffer[2].position.x = 100;
	vbuffer[2].position.y = -100;
	vbuffer[2].position.z = 0;
	vbuffer[3].position.x = 100;
	vbuffer[3].position.y = 100;
	vbuffer[3].position.z = 0;

	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	go.transform.setTranslation(0,0,2);
	go.transform.setRotation(0,0,30);
	go.transform.setScale(2,2,2);

	return go;
}

void buildScene(Scene* scene)
{
	configCamera(scene->mainCamera());

	scene->addGameObject(loadGameObject());

	scene->isDrawline = true;
}