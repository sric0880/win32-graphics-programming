#pragma once
#include "Camera.h"
#include "GameObject.h"

class Scene
{
public:
	Scene();
	~Scene();
	void drawScene();
private:
	Camera* camera;
	GameObject* object;

	Fragment* allFragments;
	int fragmentsSize;
	UINT8* frameBuffer;
	float* depthBuffer;

	// 1. modelview and projection transform
	// 2. normalize
	// 3. gouraud lighting (support ambient light and directional light)
	void processVertex(Vertex* vertex);

	//generate fragments from triangle made up with v1, v2 and v3
	// return fragments size
	// 1. clipping
	// 2. rasterization and interpolation
	int generateFragment(Vertex* v1, Vertex* v2, Vertex* v3);

	// 1. texture mapping
	// 2. per pixel lighting
	void processFragment(int size);
	void drawPixels(HDC hdc);

	void drawTriangle(HDC hdc, Vertex* buffer, UINT32 buffer_size, int* index, UINT32 index_size);
};