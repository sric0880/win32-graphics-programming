#include "stdafx.h"
#include "Geometry.h"

///////
//Draw line agorithms
void dda(HDC hdc, const Vertex& start, const Vertex& end)
{
	float dx, dy;
}

// 1. modelview and projection transform
// 2. normalize
// 3. gouraud lighting (support ambient light and directional light)
void processVertex(Vertex* vertex);
Fragment* allFragments = nullptr;
int fragmentsSize = 10000;
UINT8* frameBuffer;
float* depthBuffer;
//generate fragments from triangle made up with v1, v2 and v3
// return fragments size
// 1. clipping
// 2. rasterization and interpolation
int generateFragment(Vertex* v1, Vertex* v2, Vertex* v3);
// 1. texture mapping
// 2. per pixel lighting
void processFragment(int size);
void drawPixels(HDC hdc);

int viewPortWidth;
int viewPortHeight;

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void drawTriangle(HDC hdc, Vertex* buffer, UINT32 buffer_size, int* index, UINT32 index_size)
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

void processVertex(Vertex* vertex)
{

}

int generateFragment(Vertex* v1, Vertex* v2, Vertex* v3)
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

void processFragment(int size)
{

}
void drawPixels(HDC hdc)
{
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, viewPortWidth, viewPortHeight);
	SelectObject(mdc, bitmap);
	//FillRect(hMemDC, &ClientRect, hBrush);  //// clear the background

	//draw pixels
	int pixelsCount = viewPortWidth * viewPortHeight;
	for (int i = 0; i < viewPortWidth; ++i)
	{
		for (int j = 0; j < viewPortHeight; ++j)
		{
			int index = viewPortWidth*(viewPortHeight-j-1) + i;
			SetPixel(mdc, i, j, RGB(frameBuffer[index], frameBuffer[index+1], frameBuffer[index+2]));
		}
	}

	//swap to hdc
	BitBlt(hdc, 0, 0, viewPortWidth, viewPortHeight, mdc, 0, 0, SRCCOPY);
	//release
	DeleteDC(mdc);
	DeleteObject(bitmap);
}
