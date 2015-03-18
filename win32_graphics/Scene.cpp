#include "stdafx.h"
#include "Scene.h"

///////
//Draw line agorithms
void dda(HDC hdc, const Vertex& start, const Vertex& end)
{
	float dx, dy;
}

Scene::Scene():
	allFragments(nullptr),
	fragmentsSize(10000),
	frameBuffer(nullptr),
	depthBuffer(nullptr)
{}

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void Scene::drawTriangle(HDC hdc, Vertex* buffer, UINT32 buffer_size, int* index, UINT32 index_size)
{
	for (int i = 0; i < buffer_size; ++i)
	{
		processVertex(&buffer[i]);
	}
	int countOfTriangles = index_size / 3;
	for (int i = 0; i < countOfTriangles; ++i)
	{
		int size = generateFragment( &buffer[index[3*i]], &buffer[index[3*i+1]], &buffer[index[3*i+2]]);
		processFragment(size);
	}
	drawPixels(hdc);
}

void Scene::processVertex(Vertex* vertex)
{

}

int Scene::generateFragment(Vertex* v1, Vertex* v2, Vertex* v3)
{
	if (!allFragments)
	{
		allFragments = (Fragment*)malloc(sizeof(Fragment)*fragmentsSize);
	}
	int newFragmentsSize = 0;
	if (newFragmentsSize > fragmentsSize)
	{
		allFragments = (Fragment*)realloc(allFragments, sizeof(Fragment)*newFragmentsSize);
	}
	
	return newFragmentsSize;
}

void Scene::processFragment(int size)
{

}
void Scene::drawPixels(HDC hdc)
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
