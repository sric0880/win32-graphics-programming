#pragma once
#include "Transform.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);

	Transform transform;

	//Maybe return nullptr
	Vertex* getVertexBuffer(UINT32 size = 0);
	//Maybe return nullptr
	int* getIndexBuffer(UINT32 size = 0);

	Vertex* getVertexAt(int i);
	int getIndexAt(int i);

	int getVertexCount() const;
	int getTriangleCount() const;

private:
	//render data
	Vertex* buffer;
	UINT32 buffer_size;
	int* index;
	UINT32 index_size;
};

