#include "stdafx.h"
#include "Testcase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"

#include "OutputDebug.h"

GameObject loadQuad();
GameObject loadCube();
GameObject loadSphere();

void configCamera(Camera* camera)
{
	camera->setIsOrthProjection(true);
	camera->setFarClippingPlane(1000);
	camera->setNearClippingPlane(0.3f);
	camera->setFieldOfView(60);
	camera->setOrthBoxHeight(10.0f);

	camera->setTranslation(0,0,-1);
	camera->setRotation(0,0,0);
}

void buildScene(Scene* scene)
{
	configCamera(scene->mainCamera());

	scene->addGameObject(loadQuad());
	scene->addGameObject(loadCube());
	scene->addGameObject(loadSphere());

	scene->isDrawline = true;
}

GameObject loadQuad()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);

	vbuffer[0].position.x = -1;
	vbuffer[0].position.y = 1;
	vbuffer[0].position.z = 0;
	vbuffer[1].position.x = -1;
	vbuffer[1].position.y = -1;
	vbuffer[1].position.z = 0;
	vbuffer[2].position.x = 1;
	vbuffer[2].position.y = -1;
	vbuffer[2].position.z = 0;
	vbuffer[3].position.x = 1;
	vbuffer[3].position.y = 1;
	vbuffer[3].position.z = 0;

	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	go.transform.setTranslation(-3,0,2);
	go.transform.setRotation(30,0,0);
	go.transform.setScale(2,2,2);

	return go;
}

GameObject loadCube()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(8);
	auto ibuffer = go.getIndexBuffer(6);

	vbuffer[0].position ;

	vbuffer[0].position.x = -1;
	vbuffer[0].position.y = 1;
	vbuffer[0].position.z = 0;
	vbuffer[1].position.x = -1;
	vbuffer[1].position.y = -1;
	vbuffer[1].position.z = 0;
	vbuffer[2].position.x = 1;
	vbuffer[2].position.y = -1;
	vbuffer[2].position.z = 0;
	vbuffer[3].position.x = 1;
	vbuffer[3].position.y = 1;
	vbuffer[3].position.z = 0;
	vbuffer[4].position.x = -1;
	vbuffer[4].position.y = 1;
	vbuffer[5].position.z = 0;
	vbuffer[1].position.x = -1;
	vbuffer[1].position.y = -1;
	vbuffer[1].position.z = 0;
	vbuffer[2].position.x = 1;
	vbuffer[2].position.y = -1;
	vbuffer[2].position.z = 0;
	vbuffer[3].position.x = 1;
	vbuffer[3].position.y = 1;
	vbuffer[3].position.z = 0;

	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	go.transform.setTranslation(-4,0,2);
	go.transform.setRotation(30,0,0);
	go.transform.setScale(2,2,2);

	return go;
}

GameObject loadSphere()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);

	vbuffer[0].position.x = -1;
	vbuffer[0].position.y = 1;
	vbuffer[0].position.z = 0;
	vbuffer[1].position.x = -1;
	vbuffer[1].position.y = -1;
	vbuffer[1].position.z = 0;
	vbuffer[2].position.x = 1;
	vbuffer[2].position.y = -1;
	vbuffer[2].position.z = 0;
	vbuffer[3].position.x = 1;
	vbuffer[3].position.y = 1;
	vbuffer[3].position.z = 0;

	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	go.transform.setTranslation(-4,0,2);
	go.transform.setRotation(30,0,0);
	go.transform.setScale(2,2,2);

	return go;
}

