#include "stdafx.h"
#include "Scene.h"
#include "Geometry.h"

Scene::Scene(HDC dc):
	hdc(dc),
	allFragments(nullptr),
	fragmentsSize(10000),
	frameBuffer(nullptr),
	depthBuffer(nullptr),
	isDrawline(false)
{}

Scene::~Scene()
{
}

void Scene::drawScene()
{
	for(auto ptr_object : objects)
	{
		modelViewMatrix = camera->getViewMatrix() * ptr_object->transform.getTransform();
		projModelViewMatrix = camera->getProjectionMatrix() * modelViewMatrix;
		render(ptr_object);
	}
}

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void Scene::render(std::shared_ptr<GameObject> ptr_obj)
{
	for (int i = 0; i < ptr_obj->getVertexCount(); ++i)
	{
		processVertex(ptr_obj->getVertexAt(i));
	}
	int countOfTriangles = ptr_obj->getTriangleCount();
	for (int i = 0; i < countOfTriangles; ++i)
	{
		int size = generateFragment(
			ptr_obj->getVertexAt(ptr_obj->getIndexAt(3*i)),
			ptr_obj->getVertexAt(ptr_obj->getIndexAt(3*i+1)),
			ptr_obj->getVertexAt(ptr_obj->getIndexAt(3*i+2)));
		processFragment(size);
	}
	drawPixels();
}

void Scene::processVertex(Vertex* vertex)
{
	// projection modelview transform
	vertex->position = projModelViewMatrix * vertex->position;

	//TODO: transform normal to camera space then normalize

	//TODO: lighting per vertex
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
		c+= drawLineBresenham(allFragments+c, &allFragments[2], &allFragments[3]);

		for (int i = 0; i < c; ++i)
		{
			allFragments[i].color.x = 0;
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
		//
	}
}
void Scene::drawPixels()
{
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, camera->viewPortWidth, camera->viewPortHeight);
	SelectObject(mdc, bitmap);
	//FillRect(hMemDC, &ClientRect, hBrush);  //// clear the background

	//draw pixels
	int pixelsCount = camera->viewPortWidth * camera->viewPortHeight;
	for (int i = 0; i < camera->viewPortWidth; ++i)
	{
		for (int j = 0; j < camera->viewPortHeight; ++j)
		{
			int index = camera->viewPortWidth*(camera->viewPortHeight-j-1) + i;
			SetPixel(mdc, i, j, RGB(frameBuffer[index], frameBuffer[index+1], frameBuffer[index+2]));
		}
	}

	//swap to hdc
	BitBlt(hdc, 0, 0, camera->viewPortWidth, camera->viewPortHeight, mdc, 0, 0, SRCCOPY);
	//release
	DeleteDC(mdc);
	DeleteObject(bitmap);
}

void Scene::resizeFragments(int size)
{
	allFragments = (Fragment*)realloc(allFragments, sizeof(Fragment)*size);
	fragmentsSize = size;
}