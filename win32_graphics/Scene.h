#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Lighting.h"
#include <vector>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();
	void onKey(CameraMoving);
	void onKeyUp();
	void onMouseDragMove(int x, int y);
	void onMouseDragStart(int x, int y);
	void onMouseDragEnd(int x, int y);
	void update(float deltaTime);
	void render(RECT rect);
	void onDraw(HDC hdc);
	void onGUI(HDC hdc);
	Camera* mainCamera() { return camera.get(); }
	Lighting* mainLighting() { return lighting.get(); }
	float getActualFps();

	//void addGameObject(std::shared_ptr<GameObject> obj);
	void addGameObject(GameObject&& obj);
	//TODO: need remove an object?
	bool isDrawline;
	float fps;
private:
	float actualFps;
	std::unique_ptr<Camera> camera;
	//GameObject* object;
	std::vector<GameObject> objects;
	//Allow only one light in scene
	std::unique_ptr<Lighting> lighting;
	Matrix projModelViewMatrix;
	Matrix modelViewMatrix;

	Vertex clippedVertex[12]; // absolutely not more than 12

	//cache all vertexs in a gameobject
	Vertex* vertexBuffer;
	int vertexBufferSize;

	//cache all the fragments generated from the vertex
	Fragment* allFragments;
	int fragmentsSize;

	//UINT8* frameBuffer;
	HDC mdc;
	BYTE* frameBuffer;
	HBITMAP bitmap;
	int screenWidth;
	int screenHeight;
	float* depthBuffer;
	int depthbufferSize;
	bool isFirstDraw;

	// 1. modelview and projection transform
	// 2. normalize
	// 3. gouraud lighting (support ambient light and directional light)
	void processVertex(const Vertex* input, Vertex* output);

	//generate fragments from triangle made up with v1, v2 and v3
	// return fragments size
	// 1. clipping
	// 2. rasterization and interpolation
	int generateFragment(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	// 1. texture mapping
	// 2. per pixel lighting
	void processFragment(int size, const Texture2D* tex);
	void depthTest(int size);

	void render(const GameObject& obj);
	bool resizeFragments(int size);
	bool resizeVertexBuffer(int size);

	void initFrameBuffer(HDC hdc);
	void initDepthBuffer();
	void initVertexBuffer();
	void clearFrameBuffer();
	void clearDepthBuffer();
	void releaseFrameBuffer();
	void releaseDepthBuffer();

	static inline int bufferIndex(int x, int y, int w);
	//upside down
	static inline int windowCoordToBufferIndex(int x, int y, int w, int h);

	//Mouse move data
	int startMouseX;
	int startMouseY;
};