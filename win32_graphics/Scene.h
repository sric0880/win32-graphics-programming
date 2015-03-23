#pragma once
#include "Camera.h"
#include "GameObject.h"
#include <vector>
#include <memory>

class Scene
{
public:
	Scene();
	~Scene();
	void drawScene(HDC hdc, int w, int h);
	Camera* mainCamera();
	//void addGameObject(std::shared_ptr<GameObject> obj);
	void addGameObject(GameObject&& obj);
	//TODO: need remove an object?
	Vector lightDir; // direction light in camera space
	Matrix normalMatrix;  // normal transform to camera space
	bool isDrawline;
private:
	std::unique_ptr<Camera> camera;
	//GameObject* object;
	std::vector<GameObject> objects;
	Matrix projModelViewMatrix;
	Matrix modelViewMatrix;

	//cache all vertexs in a gameobject
	Vertex* vertexBuffer;
	int vertexBufferSize;

	//cache all the fragments generated from the vertex
	Fragment* allFragments;
	int fragmentsSize;

	UINT8* frameBuffer;
	float* depthBuffer;

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
	void processFragment(int size);
	void depthTest(int size);
	void drawPixels(HDC hdc);

	void render(const GameObject& obj);
	void resizeFragments(int size);
	void resizeVertexBuffer(int size);

	void initFrameBuffer();
	void initDepthBuffer();
	void initVertexBuffer();
	void clearFrameBuffer();
	void clearDepthBuffer();
	void releaseFrameBuffer();
	void releaseDepthBuffer();

	static inline int bufferIndex(int x, int y, int viewportWidth, int viewportHeight);
	//upside down
	static inline int windowCoordToBufferIndex(int x, int y, int viewportWidth, int viewportHeight);
};