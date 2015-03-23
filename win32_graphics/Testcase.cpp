#include "stdafx.h"
#include "Testcase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"

#include "OutputDebug.h"

GameObject Quad();
GameObject Cube();
GameObject Sphere();

void configCamera(Camera* camera)
{
	//camera->setIsOrthProjection(true);
	camera->setIsOrthProjection(false);
	camera->setFarClippingPlane(1000);
	camera->setNearClippingPlane(0.3f);
	camera->setFieldOfView(60);
	camera->setOrthBoxHeight(10.0f);

	camera->setTranslation(0,0,-4);
	camera->setRotation(0,0,0);
}

void buildScene(Scene* scene)
{
	configCamera(scene->mainCamera());

	scene->addGameObject(Quad());
	scene->addGameObject(Cube());
	//scene->addGameObject(Sphere());

	scene->isDrawline = false;
}

GameObject Quad()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);

	vbuffer[0] = { { -1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[1] = { { -1, -1, 0 }, { 1, 0, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[2] = { { 1, -1, 0 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} }; //red
	vbuffer[3] = { { 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} }; //green

	int index[] = {0,1,2, 0,2,3};
	memcpy(ibuffer, index, sizeof(int) * 6);

	go.transform.setTranslation(0,0,2);
	go.transform.setRotation(0,0,20);
	go.transform.setScale(2,2,2);

	return go;
}

GameObject Cube()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(8);
	auto ibuffer = go.getIndexBuffer(36);

	// position // color // texture coord // normal
	vbuffer[0] = { { 1, 1, -1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[1] = { { -1, 1, -1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[2] = { { -1, -1, -1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} }; //red
	vbuffer[3] = { { 1, -1, -1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} }; //green
	vbuffer[4] = { { 1, -1, 1 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[5] = { { 1, 1, 1 }, { 1, 0, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[6] = { { -1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} };
	vbuffer[7] = { { -1, -1, 1 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 }, {0,0,0,0} }; //blue

	int index[] = { 0, 1, 2, 2, 3, 0,   // 36 of indices
		0, 3, 4, 4, 5, 0,
		0, 5, 6, 6, 1, 0,
		1, 6, 7, 7, 2, 1,
		7, 4, 3, 3, 2, 7,
		4, 7, 6, 6, 5, 4 };

	memcpy(ibuffer, index, sizeof(int) * 36);

	go.transform.setTranslation(0, 0, 2);
	go.transform.setRotation(45,45,45);
	go.transform.setScale(1,1,1);

	return go;
}

GameObject Sphere()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);

	go.transform.setTranslation(-4,0,2);
	go.transform.setRotation(30,0,0);
	go.transform.setScale(2,2,2);

	return go;
}

