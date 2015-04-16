#include "stdafx.h"
#include "Scene.h"
#include "Geometry.h"
#include "Texture2D.h"

#include "OutputDebug.h"
#include <time.h>
#include <cassert>

Scene::Scene():
	allFragments(nullptr),
	vertexBuffer(nullptr),
	vertexBufferSize(1000),
	fragmentsSize(10000),
	frameBuffer(nullptr),
	//bitmap(0),
	depthBuffer(nullptr),
	isDrawline(false),
	camera(new Camera()),
	lighting(new Lighting()),
	fps(60),
	actualFps(60),
	isFirstDraw(true)
{
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	depthbufferSize = screenWidth * screenHeight * sizeof(float);
	initDepthBuffer();
}

Scene::~Scene()
{
	releaseFrameBuffer();
	releaseDepthBuffer();
	if (allFragments) free(allFragments);
	if (vertexBuffer) free(vertexBuffer);
}


void Scene::onKey(CameraMoving key)
{
	camera->movingDir = key;
	camera->movingSpeed = 3.0f; // Customize
}

void Scene::onKeyUp()
{
	camera->movingSpeed = 0;
}

void Scene::onMouseDragMove(int x, int y)
{
	float ry = (float)(y - startMouseY) / camera->getViewportHeight();
	ry *= camera->getFieldOfView();

	float rx = (float)(x - startMouseX) / camera->getViewportWidth();
	rx *= camera->getHorizonFieldOfView();

	Vector eulerAngle = camera->getTransform().getEulerAngle();
	Vector newAngle(eulerAngle.x + ry, eulerAngle.y + rx, eulerAngle.z);

	camera->setRotation(newAngle.x, newAngle.y, newAngle.z);
	
	startMouseX = x;
	startMouseY = y;
}

void Scene::onMouseDragStart(int x, int y)
{
	startMouseX = x;
	startMouseY = y;
}
void Scene::onMouseDragEnd(int x, int y)
{
	startMouseX = 0;
	startMouseY = 0;
}

void Scene::onGUI(HDC hdc)
{
	WCHAR str[20];
	swprintf(str, L"FPS: %f", actualFps);
	TextOut(hdc, 10, 10, str, wcslen(str));
}

float Scene::getActualFps()
{
	return actualFps;
}

void Scene::addGameObject(GameObject&& obj)
{
	objects.emplace_back(std::forward<GameObject&&>(obj));
}

void Scene::update(float deltaTime)
{
	//update camera
	if (camera->movingSpeed > 0)
	{
		switch (camera->movingDir)
		{
		case Forward:
			camera->moveForward(deltaTime);
			break;
		case Backward:
			camera->moveBackward(deltaTime);
			break;
		case Left:
			camera->moveLeft(deltaTime);
			break;
		case Right:
			camera->moveRight(deltaTime);
			break;
		}
	}

	//update objects
	if (deltaTime != 0)
		actualFps = 1.0f / deltaTime;
	for (auto& gameobject : objects)
	{
		if (gameobject.updateFunc)
			gameobject.updateFunc(&gameobject, deltaTime);
	}
}

void Scene::render(RECT rect)
{
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	camera->setViewPortWidth(w);
	camera->setViewPortHeight(h);
	clearFrameBuffer();
	clearDepthBuffer();
	for(auto& ptr_object : objects)
	{
		Matrix transform = ptr_object.transform.getTransform();
#ifdef _DEBUG
		print("Object tranform matrix: \r\n");
		transform.log();
#endif
		modelViewMatrix = camera->getViewMatrix() * transform;
		lighting->setNormalMatrix(modelViewMatrix);
		lighting->updateViewMatrix(&camera->getViewMatrix());
#ifdef _DEBUG
		print("model view matrix: \r\n");
		modelViewMatrix.log();
#endif
		projModelViewMatrix = camera->getProjectionMatrix() * modelViewMatrix;
#ifdef _DEBUG
		print("total projmodelview matrix: \r\n");
		projModelViewMatrix.log();
#endif
		render(ptr_object);
	}
}

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void Scene::render(const GameObject& obj) //gameobject must be const
{
	int vertexCount = obj.getVertexCount();
	if (!vertexBuffer)
	{
		vertexBuffer = (Vertex*)malloc(sizeof(Vertex)* vertexCount);
		vertexBufferSize = vertexCount;
	}else if (vertexBufferSize < vertexCount)
	{
		resizeVertexBuffer(vertexCount);
		if (vertexBufferSize == 0) return;
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		processVertex(obj.getVertexAt(i), vertexBuffer + i);
	}
	int countOfTriangles = obj.getTriangleCount();
	for (int i = 0; i < countOfTriangles; ++i)
	{
		Vertex* v1 = vertexBuffer + obj.getIndexAt(3 * i);
		Vertex* v2 = vertexBuffer + obj.getIndexAt(3 * i + 1);
		Vertex* v3 = vertexBuffer + obj.getIndexAt(3 * i + 2);
		//backface culling
		if (isBackface(v1, v2, v3)) continue;

		//start clipping
		int count = clippingTriangle(v1, v2, v3, clippedVertex);
		count -= 2;
		for (int j = 0; j < count; ++j)
		{
			int size = generateFragment(clippedVertex[0], clippedVertex[j+1], clippedVertex[j+2] );
			processFragment(size, obj.getTexture2D());
			depthTest(size);
		}
	}
}

void Scene::processVertex(const Vertex* input, Vertex* output)
{
	// projection modelview transform
	output->eye = -1 * (modelViewMatrix * input->position);
	output->position = projModelViewMatrix * input->position;

	//first normalize the normal
	// then transform normal to camera space (!! CANNOT USE MODELVIEW MATRIX!!)
	//  then normalize it
	output->normal = input->normal;
	output->normal.normalize();
	output->normal = lighting->getNormalMatrix() * output->normal;
	output->normal.normalize();

	//lighting per vertex
	if (!lighting->isPerPixelLighting())
	{
		Vector E = output->eye;
		E.normalize();
		output->color = lighting->calculateLighting(output->normal, E);
	}
	else
	{
		output->color = input->color;
	}

	output->texCoord = input->texCoord;
}

int Scene::generateFragment(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	if (!allFragments)
	{
		allFragments = (Fragment*)malloc(sizeof(Fragment)*fragmentsSize);
		if (!allFragments) return 0;
	}

	// transform to view port
	Vector pos1 = camera->getViewportMatrix() * v1.position;
	Vector pos2 = camera->getViewportMatrix() * v2.position;
	Vector pos3 = camera->getViewportMatrix() * v3.position;

	//Draw line
	if (isDrawline)
	{
		int count = max( fabs(pos1.x - pos2.x), fabs(pos1.y - pos2.y));
		count += max( fabs(pos1.x - pos3.x), fabs(pos1.y - pos3.y));
		count += max( fabs(pos3.x - pos2.x), fabs(pos3.y - pos2.y));
		count += 3; // for safe
		if (count > fragmentsSize)
		{
			resizeFragments(count);
			if (fragmentsSize == 0) return 0;
		}
		allFragments[0].x = pos1.x;
		allFragments[0].y = pos1.y;
		allFragments[1].x = pos2.x;
		allFragments[1].y = pos2.y;
		allFragments[2].x = pos3.x;
		allFragments[2].y = pos3.y;

		int c = 3;
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[1]);
		c+= drawLineBresenham(allFragments+c, &allFragments[1], &allFragments[2]);
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[2]);
		//set all the line color to black
		for (int i = 0; i < c; ++i)
		{
			allFragments[i].color.x = 1;
			allFragments[i].color.y = 1;
			allFragments[i].color.z = 1;
			allFragments[i].color.w = 0;
		}
		return c;
	}
	else
	//Draw triangles
	// rasterization and interpolation
	{
		//Rasterization
		FillData data;
		scanTriangle(&data, pos1, pos2, pos3);
		if (data.fragmentsCount == 0) return 0;
		if(data.fragmentsCount > fragmentsSize)
		{
			resizeFragments(data.fragmentsCount);
			if (fragmentsSize == 0) return 0;
		}
		int c = 0;
		for(auto line : data.lines)
		{
			for (int j = line.left; j <= line.right; ++j)
			{
				allFragments[c].x = j;
				allFragments[c].y = line.y;
				++c;
			}
		}

		//Interpolation
		Matrix mat;
		mat.m[0].x = pos1.x;
		mat.m[1].x = pos2.x;
		mat.m[2].x = pos3.x;
		mat.m[0].y = pos1.y;
		mat.m[1].y = pos2.y;
		mat.m[2].y = pos3.y;
		mat.m[0].z = 1;
		mat.m[1].z = 1;
		mat.m[2].z = 1;
		Matrix interpolationMatrix = ~mat;
		Vector e1 = v1.eye; e1.normalize();
		Vector e2 = v2.eye; e2.normalize();
		Vector e3 = v3.eye; e3.normalize();
		for(int i = 0; i < c; ++i)
		{
			Vector interp = interpolationMatrix * Vector(allFragments[i].x, allFragments[i].y, 1);
			clerp(0, 1, interp); //little error
			if (c == 1)
			{
				interp.x = interp.y = interp.z = 1.0f/3.0f;
			}
			//Color interpolation
			allFragments[i].color = v1.color * interp.x + v2.color * interp.y + v3.color * interp.z;
			clerp(0, 1, allFragments[i].color);
			
			//Normal interpolation
			allFragments[i].normal = v1.normal * interp.x + v2.normal * interp.y + v3.normal * interp.z;
			
			//Depth interpolation
			allFragments[i].depth = pos1.z * interp.x + pos2.z * interp.y + pos3.z * interp.z;
			if (allFragments[i].depth < 0)
			{
				print("-----------------");
				interp.log();
				print("fragment:%d of %d, x:%d, y:%d, depth: %f", i, c, allFragments[i].x, allFragments[i].y, allFragments[i].depth);
				pos1.log();
				pos2.log();
				pos3.log();
			}

			//Texture Coord interpolation
			if (camera->getIsOrthProjection())
			{
				allFragments[i].texCoord = (v1.texCoord* interp.x + v2.texCoord* interp.y + v3.texCoord* interp.z);
			}
			else 
			{
				float zr = -1.0f / (1.0f/v1.eye.z*interp.x + 1.0f/v2.eye.z*interp.y + 1.0f/v3.eye.z*interp.z);
				allFragments[i].texCoord = (v1.texCoord * (-1.0f/v1.eye.z)* interp.x + v2.texCoord * (-1.0f/v2.eye.z) * interp.y + v3.texCoord* (-1.0f/v3.eye.z) * interp.z) * zr;
			}
			clerp(0, 1, allFragments[i].texCoord);

			//Eye interpolation
			allFragments[i].eye = e1 * interp.x + e2 * interp.y + e3 * interp.z;
		}
		return c;
	}
}

void Scene::processFragment(int size, const Texture2D* tex)
{
	for(int i = 0; i < size; ++i)
	{
		Fragment& frag = allFragments[i];

		//Lighting per pixel
		if (lighting->isPerPixelLighting())
		{
			frag.normal.normalize();
			frag.eye.normalize();
			frag.color = lighting->calculateLighting(frag.normal, frag.eye);
		}
		//Texture mapping
		if (!tex->isEmpty())
		{
			Vector sample = tex->sampleTexture2D(frag.texCoord.x, frag.texCoord.y);
			frag.color.x *= sample.x;
			frag.color.y *= sample.y;
			frag.color.z *= sample.z;
		}
	}
}

void Scene::depthTest(int size)
{
	int s = 3 * screenWidth*screenHeight;
	for (int i = 0; i < size; ++i)
	{
		auto& frame = allFragments[i];
		int index_ = bufferIndex(frame.x, frame.y, screenWidth);
		int index = 3*index_;
		if (index + 2>= s || index_ >= depthbufferSize)
			print("%d<%d, %d<%d", index_, depthbufferSize, index, s);
		if (isDrawline) //not need test depth
		{
			//update to frame buffer
			frameBuffer[index] = allFragments[i].color.z * 0xff;
			frameBuffer[index + 1] = allFragments[i].color.y * 0xff;
			frameBuffer[index + 2] = allFragments[i].color.x * 0xff;
		}
		else if (depthBuffer[index_] > allFragments[i].depth)
		{
			depthBuffer[index_] = allFragments[i].depth;

			//update to frame buffer
			frameBuffer[index] = allFragments[i].color.z * 0xff;
			frameBuffer[index + 1] = allFragments[i].color.y * 0xff;
			frameBuffer[index + 2] = allFragments[i].color.x * 0xff;
		}
	}
}

void Scene::resizeFragments(int size)
{
	auto temp = (Fragment*)realloc(allFragments, sizeof(Fragment)*size);
	if (temp) allFragments = temp;
	else
	{
		free(allFragments);
		fragmentsSize = 0;
		allFragments = nullptr;
		return;
	}
	fragmentsSize = size;
}
void Scene::resizeVertexBuffer(int size)
{
	auto temp = (Vertex*)realloc(vertexBuffer, sizeof(Vertex)* size);
	if (temp) vertexBuffer = temp;
	else
	{
		free(vertexBuffer);
		vertexBufferSize = 0;
		vertexBuffer = nullptr;
	}
	vertexBufferSize = size;
}

void Scene::onDraw(HDC hdc)
{
	if (isFirstDraw)
	{
		initFrameBuffer(hdc);
		isFirstDraw = false;
	}
	int w = camera->getViewportWidth();
	int h = camera->getViewportHeight();
	BitBlt(hdc, 0, 0, w, h, mdc, 0, screenHeight - h, SRCCOPY);
}

void Scene::initFrameBuffer(HDC hdc)
{
	mdc = CreateCompatibleDC(hdc);
	BITMAPINFO bmInfo;
	memset(&bmInfo, 0, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = screenWidth;
	bmInfo.bmiHeader.biHeight = screenHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bitmap = CreateDIBSection(mdc, &bmInfo, DIB_RGB_COLORS, (void**)&frameBuffer, NULL, 0);
	//bitmap = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);
	assert(frameBuffer != nullptr);
	SelectObject(mdc, bitmap);
}
void Scene::initDepthBuffer()
{
	if (!depthBuffer)
		depthBuffer = (float*) malloc(depthbufferSize);
}
void Scene::clearFrameBuffer()
{
	if (frameBuffer)
		memset(frameBuffer, 0, 3 * screenWidth*screenHeight);
}
void Scene::clearDepthBuffer()
{
	int c = depthbufferSize / 4;
	if (depthBuffer)
	{
		for (int i = 0; i < c; ++i) depthBuffer[i] = 1.0f;
	}
}
void Scene::releaseFrameBuffer()
{
	if (!isFirstDraw)
	{
		DeleteDC(mdc);
		DeleteObject(bitmap);
	}
}
void Scene::releaseDepthBuffer()
{
	if(depthBuffer)
		free(depthBuffer);
	depthBuffer = nullptr;
}

inline int Scene::bufferIndex(int x, int y, int w)
{
	return w * y + x ;
}

inline int Scene::windowCoordToBufferIndex(int x, int y, int w, int h)
{
	return w * (h - y) + x;
}