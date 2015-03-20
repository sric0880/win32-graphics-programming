#include "stdafx.h"
#include "Scene.h"
#include "Geometry.h"

#include "OutputDebug.h"

Scene::Scene():
	allFragments(nullptr),
	vertexBuffer(nullptr),
	vertexBufferSize(1000),
	fragmentsSize(1000),
	frameBuffer(nullptr),
	depthBuffer(nullptr),
	isDrawline(false),
	camera(new Camera())
{
	initFrameBuffer();
	initDepthBuffer();
}

Scene::~Scene()
{
	releaseFrameBuffer();
	releaseDepthBuffer();
	if (allFragments) free(allFragments);
	if (vertexBuffer) free(vertexBuffer);
}

Camera* Scene::mainCamera()
{
	return camera.get();
}

void Scene::addGameObject(GameObject&& obj)
{
	objects.emplace_back(std::forward<GameObject&&>(obj));
}
void Scene::drawScene(HDC hdc, int w, int h)
{
	camera->setViewPortWidth(w);
	camera->setViewPortHeight(h);
	clearFrameBuffer();
	clearDepthBuffer();
	for(auto& ptr_object : objects)
	{
		modelViewMatrix = camera->getViewMatrix() * ptr_object.transform.getTransform();
		projModelViewMatrix = camera->getProjectionMatrix() * modelViewMatrix;
		render(ptr_object);
		drawPixels(hdc);
	}
}

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void Scene::render(const GameObject& ptr_obj) //gameobject must be const
{
	int vertexCount = ptr_obj.getVertexCount();
	if (!vertexBuffer)
	{
		vertexBuffer = (Vertex*)malloc(sizeof(Vertex)* vertexCount);
		vertexBufferSize = vertexCount;
	}else if (vertexBufferSize < vertexCount)
	{
		resizeVertexBuffer(vertexCount);
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		processVertex(ptr_obj.getVertexAt(i), vertexBuffer + i);
	}
	int countOfTriangles = ptr_obj.getTriangleCount();
	for (int i = 0; i < countOfTriangles; ++i)
	{
		int size = generateFragment(
			vertexBuffer + ptr_obj.getIndexAt(3*i),
			vertexBuffer + ptr_obj.getIndexAt(3*i+1),
			vertexBuffer + ptr_obj.getIndexAt(3*i+2));
		print("fragments size: %d", size);
		processFragment(size);
		depthTest(size);
	}
}

void Scene::processVertex(const Vertex* input, Vertex* output)
{
	// projection modelview transform
	output->position = projModelViewMatrix * input->position;

	//TODO: transform normal to camera space then normalize

	//TODO: lighting per vertex

	output->color = input->color;
	output->normal = input->normal;
	output->texCoord = input->texCoord;
}

int Scene::generateFragment(Vertex* v1, Vertex* v2, Vertex* v3)
{
	if (!allFragments)
	{
		allFragments = (Fragment*)malloc(sizeof(Fragment)*fragmentsSize);
	}
	//TODO: clipping transformation

	// transform to view port
	v1->position = camera->getViewportMatrix() * v1->position;
	v2->position = camera->getViewportMatrix() * v2->position;
	v3->position = camera->getViewportMatrix() * v3->position;

	//Draw line
	if (isDrawline)
	{
		int count = max( fabs(v1->position.x - v2->position.x), fabs(v1->position.y - v2->position.y));
		count += max( fabs(v1->position.x - v3->position.x), fabs(v1->position.y - v3->position.y));
		count += max( fabs(v3->position.x - v2->position.x), fabs(v3->position.y - v2->position.y));
		if (count > fragmentsSize)
		{
			resizeFragments(count);
		}
		allFragments[0].x = v1->position.x;
		allFragments[0].y = v1->position.y;
		allFragments[1].x = v2->position.x;
		allFragments[1].y = v2->position.y;
		allFragments[2].x = v3->position.x;
		allFragments[2].y = v3->position.y;
		int c = 3;
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[1]);
		c+= drawLineBresenham(allFragments+c, &allFragments[1], &allFragments[2]);
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[2]);

		for (int i = 0; i < c; ++i)
		{
			allFragments[i].color.x = 1;
			allFragments[i].color.y = 0;
			allFragments[i].color.z = 0;
			allFragments[i].color.w = 0;
		}

		return c;
	}
	else
	//Draw triangles
	// rasterization and interpolation
	{
		//TODO: 1. culling ±³ÃæÏû³ý
		
		//2. Rasterization
		FillData data;
		scanTriangle(&data, v1->position, v2->position, v3->position);
		if(data.fragmentsCount > fragmentsSize)
		{
			resizeFragments(data.fragmentsCount);
		}
		int c = 0;
		int y = data.ymin;
		for(auto line : data.lines)
		{
			for (int j = line.left; j <= line.right; ++j)
			{
				allFragments[c].y = y;
				allFragments[c].x = j;
				++c;
			}
			++y;
		}

		//3. Interpolation
		Matrix mat;
		mat.m[0].x = v1->position.x;
		mat.m[1].x = v2->position.x;
		mat.m[2].x = v3->position.x;
		mat.m[0].y = v1->position.y;
		mat.m[1].y = v2->position.y;
		mat.m[2].y = v3->position.y;
		mat.m[0].z = 1;
		mat.m[1].z = 1;
		mat.m[2].z = 1;
		for(int i = 0; i < c; ++i)
		{
			Vector v(allFragments[i].x, allFragments[i].y, 1);
			Vector interp = mat * v; //FIXME: reverse mat
			allFragments[i].color = v1->color * interp.x + v2->color * interp.y + v3->color * interp.z;
			allFragments[i].normal = v1->normal * interp.x + v2->normal * interp.y + v3->normal * interp.z;
			allFragments[i].texCoord = v1->texCoord * interp.x + v2->texCoord * interp.y + v3->texCoord * interp.z;
			allFragments[i].depth = v1->position.z * interp.x + v2->position.z * interp.y + v3->position.z * interp.z;
		}
		return c;
	}
}

void Scene::processFragment(int size)
{
	for(int i = 0; i < size; ++i)
	{
		//TODO: Texture mapping

		//TODO: Lighting per pixel

		//allFragments[0].color =  //	
	}
}

void Scene::depthTest(int size)
{
	int w = camera->getViewportWidth();
	int h = camera->getViewportHeight();
	for (int i = 0; i < size; ++i)
	{
		auto& frame = allFragments[i];
		int index = fromPortviewCoordToBufferIndex(frame.x, frame.y, w, h);
		//FIXME: x and y is not right!!!
		if(allFragments[i].depth < depthBuffer[index])
		{
			depthBuffer[index] = allFragments[i].depth;
			//update to frame buffer
			frameBuffer[3*index] = allFragments[i].color.x * 0xff;
			frameBuffer[3*index + 1] = allFragments[i].color.y * 0xff;
			frameBuffer[3*index + 2] = allFragments[i].color.z * 0xff;
		}
	}
}
void Scene::drawPixels(HDC hdc)
{
	HDC mdc = CreateCompatibleDC(hdc);
	int viewportWidth = camera->getViewportWidth();
	int viewportHeight = camera->getViewportHeight();
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, viewportWidth, viewportHeight);
	SelectObject(mdc, bitmap);
	//FillRect(hMemDC, &ClientRect, hBrush);  //// clear the background

	//draw pixels
	int pixelsCount = viewportWidth * viewportHeight;
	for (int i = 0; i < viewportWidth; ++i)
	{
		for (int j = 0; j < viewportHeight; ++j)
		{
			int index = fromPortviewCoordToBufferIndex(i, j, viewportWidth, viewportHeight);
			SetPixel(mdc, i, j, RGB(frameBuffer[3*index], frameBuffer[3*index+1], frameBuffer[3*index+2]));
		}
	}

	//swap to hdc
	BitBlt(hdc, 0, 0, viewportWidth, viewportHeight, mdc, 0, 0, SRCCOPY);
	//release
	DeleteDC(mdc);
	DeleteObject(bitmap);
}

void Scene::resizeFragments(int size)
{
	allFragments = (Fragment*)realloc(allFragments, sizeof(Fragment)*size);
	fragmentsSize = size;
}
void Scene::resizeVertexBuffer(int size)
{
	vertexBuffer = (Vertex*)realloc(vertexBuffer, sizeof(Vertex)* size);
	vertexBufferSize = size;
}

const int framebufferSize = 1680*1050*3;
const int depthbufferSize = 1680*1050*sizeof(float);

void Scene::initFrameBuffer()
{
	if (!frameBuffer)
		frameBuffer = (UINT8*) malloc( framebufferSize );
}
void Scene::initDepthBuffer()
{
	if (!depthBuffer)
		depthBuffer = (float*) malloc(depthbufferSize);
}
void Scene::clearFrameBuffer()
{
	if(frameBuffer)
		memset(frameBuffer, 0, framebufferSize);
}
void Scene::clearDepthBuffer()
{
	if(depthBuffer)
		memset(depthBuffer, -1, depthbufferSize);
}
void Scene::releaseFrameBuffer()
{
	if(frameBuffer)
		free(frameBuffer);
	frameBuffer = nullptr;
}
void Scene::releaseDepthBuffer()
{
	if(depthBuffer)
		free(depthBuffer);
	depthBuffer = nullptr;
}

int Scene::fromPortviewCoordToBufferIndex(int x, int y, int viewportWidth, int viewportHeight)
{
	return viewportWidth* (viewportHeight - y -1) + x;
}