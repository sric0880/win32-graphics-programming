#include "stdafx.h"
#include "Testcase.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Texture2D.h"

#include "OutputDebug.h"
#include "resource.h"

GameObject Quad();
GameObject Cube();
GameObject Sphere();

void configCamera(Camera* camera)
{
	//camera->setIsOrthProjection(true);
	camera->setIsOrthProjection(false);
	camera->setFarClippingPlane(1000);
	camera->setNearClippingPlane(0.3f);
	camera->setFieldOfView(30);
	camera->setOrthBoxHeight(10.0f);

	camera->setTranslation(0,0,-1);
	camera->setRotation(0,0,0);
}

void configLighting(Lighting* lighting)
{
	lighting->setPerPixelLighting(false);
	lighting->setDirection(Vector(0,0,1));
	lighting->setAmbient(Vector(0.2, 0.2, 0.2));
	lighting->setDiffuse(Vector(0.8, 0.8, 0.8));
	lighting->setSpecular(Vector(1.0, 1.0, 1.0));
	lighting->setShininess(30);
}

void buildScene(Scene* scene)
{
	configCamera(scene->mainCamera());
	configLighting(scene->mainLighting());

	scene->addGameObject(Cube());
	scene->addGameObject(Quad());
	scene->addGameObject(Sphere());

	scene->isDrawline = false;
}

GameObject Quad()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(4);
	auto ibuffer = go.getIndexBuffer(6);
	auto tex = go.getTexture2D();
	tex->loadTexture2D(MAKEINTRESOURCE(IDB_BITMAP1));

	vbuffer[0] = { { -1, 1, 0 }, { 1, 1, 1, 0 }, { 0, 1, 0, 0 }, {0,0,-1,0} };
	vbuffer[1] = { { -1, -1, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,-1,0} };
	vbuffer[2] = { { 1, -1, 0 }, { 1, 1, 1, 0 }, { 1, 0, 0, 0 }, {0,0,-1,0} }; //red
	vbuffer[3] = { { 1, 1, 0 }, { 1, 1, 1, 0 }, { 1, 1, 0, 0 }, {0,0,-1,0} }; //green

	int index[] = {0,1,2, 0,2,3};
	memcpy(ibuffer, index, sizeof(int) * 6);

	go.transform.setTranslation(-3,0,5);
	go.transform.setRotation(10,0,0);
	go.transform.setScale(2,2,2);

	return go;
}

GameObject Cube()
{
	GameObject go;
	auto vbuffer = go.getVertexBuffer(24);
	auto ibuffer = go.getIndexBuffer(36);

	 //position // color // texture coord // normal
	//front
	vbuffer[0] = { { 1, 1, -1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,-1,0} };
	vbuffer[1] = { { -1, 1, -1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,0,-1,0} };
	vbuffer[2] = { { -1, -1, -1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,0,-1,0} };
	vbuffer[3] = { { 1, -1, -1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,0,-1,0} };
	//back
	vbuffer[4] = { { 1, -1, 1 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,0,1,0} };
	vbuffer[5] = { { 1, 1, 1 }, { 1, 0, 1, 0 }, { 0, 0, 0, 0 }, {0,0,1,0} };
	vbuffer[6] = { { -1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,0,1,0} };
	vbuffer[7] = { { -1, -1, 1 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 }, {0,0,1,0} };
	//left
	vbuffer[8] = { { -1, 1, -1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {-1,0,0,0} };
	vbuffer[9] = { { -1, 1, 1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {-1,0,0,0} };
	vbuffer[10] = { { -1, -1, 1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {-1,0,0,0} };
	vbuffer[11] = { { -1, -1, -1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, {-1,0,0,0} };
	//right
	vbuffer[12] = { { 1, 1, -1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {1,0,0,0} };
	vbuffer[13] = { { 1, -1, -1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {1,0,0,0} };
	vbuffer[14] = { { 1, -1, 1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 0, 0, 0 } };
	vbuffer[15] = { { 1, 1, 1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {1,0,0,0} };
	//up
	vbuffer[16] = { { 1, 1, -1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,1,0,0} };
	vbuffer[17] = { { 1, 1, 1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,1,0,0} };
	vbuffer[18] = { { -1, 1, 1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,1,0,0} };
	vbuffer[19] = { { -1, 1, -1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 1, 0, 0 } };
	//down
	vbuffer[20] = { { -1, -1, 1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,-1,0,0} };
	vbuffer[21] = { { 1, -1, 1 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, {0,-1,0,0} };
	vbuffer[22] = { { 1, -1, -1 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 }, {0,-1,0,0} };
	vbuffer[23] = { { -1, -1, -1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, {0,-1,0,0} };

	 // 36 of indices
	int index[] = { 0, 1, 2, 2, 3, 0,  //front
		12, 13, 14, 14, 15, 12, //right
		16, 17, 18, 18, 19, 16, //up
		8, 9, 10, 10, 11, 8, //left
		20, 21, 22, 22, 23, 20, //down
		4, 7, 6, 6, 5, 4 }; //back

	memcpy(ibuffer, index, sizeof(int) * 36);

	go.transform.setTranslation(1, 0, 6);
	go.transform.setRotation(-40,30,70);
	go.transform.setScale(1,1,1);

	return go;
}

GameObject Sphere()
{
	GameObject go;
	const float PI = 3.1415926535f;
	const float PI2 = 6.2831853f;
	const int vertexCountPerCircle = 18;
	const int countOfCircles = 37;
	const int vertexCount = vertexCountPerCircle * countOfCircles - 2*(countOfCircles-1);
	const int indexCount = (vertexCountPerCircle-2) * (countOfCircles-1) * 6;
	const float angleVerticle = PI / (vertexCountPerCircle-1);
	const float angleHorizon = PI2 / (countOfCircles-1);

	auto vbuffer = go.getVertexBuffer(vertexCount);
	auto ibuffer = go.getIndexBuffer(indexCount);

	auto tex = go.getTexture2D();
	tex->loadTexture2D(MAKEINTRESOURCE(IDB_BITMAP2));

	vbuffer[0] = { { 0, -1, 0 }, { 0.75, 0.75, 0.75 }, { 0, 0, 0, 0 }, { 0, -1, 0 } };
	vbuffer[1] = { { 0, 1, 0 }, { 0.75, 0.75, 0.75 }, { 0, 1, 0, 0 }, { 0, 1, 0 } };

	int c = 2;
	for (int i = 0; i < countOfCircles; ++i)
	{
		float zs = sin(i * angleHorizon);
		float xc = cos(i * angleHorizon);
		for (int j = 1; j < vertexCountPerCircle-1; ++j)
		{
			float a = j * angleVerticle - PI/2;
			float y = sin(a);
			float temp = cos(a);
			float z = temp * zs;
			float x = temp * xc;
			vbuffer[c].position = { x, y, z };
			vbuffer[c].color = { 0, 0, 0 };
			vbuffer[c].normal = { x, y, z };
			vbuffer[c].texCoord = { (float)i/countOfCircles, (float)j / (vertexCountPerCircle-1), 0 };
			++c;
		}
	}

	c = 0;
	for (int i = 0; i < countOfCircles - 1; ++i)
	{
		int startIndex = i *(vertexCountPerCircle - 2) + 2;
		int nextStartIndex = (i+1)*(vertexCountPerCircle - 2) + 2;
		for (int j = 0; j < vertexCountPerCircle - 1; ++j)
		{
			if (j == 0)
			{
				ibuffer[c++] = 0;
				ibuffer[c++] = nextStartIndex;
				ibuffer[c++] = startIndex;
			}
			else if (j == vertexCountPerCircle - 2)
			{
				ibuffer[c++] = startIndex + j - 1;
				ibuffer[c++] = nextStartIndex + j - 1;
				ibuffer[c++] = 1;
			}
			else
			{
				int i1 = startIndex + j - 1;
				int i2 = nextStartIndex + j - 1;
				int i3 = nextStartIndex + j;
				int i4 = startIndex + j;
				ibuffer[c++] = i1;
				ibuffer[c++] = i2;
				ibuffer[c++] = i3;

				ibuffer[c++] = i3;
				ibuffer[c++] = i4;
				ibuffer[c++] = i1;
			}
		}
	}
	

	go.transform.setTranslation(3,0,4);
	go.transform.setRotation(30,90,30);
	go.transform.setScale(1,1,1);

	return go;
}

