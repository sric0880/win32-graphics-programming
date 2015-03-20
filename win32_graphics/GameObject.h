#pragma once
#include "Transform.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);

	GameObject(GameObject&&);
	GameObject& operator= (GameObject&& obj);
	
	Transform transform;

	//Maybe return nullptr
	Vertex* getVertexBuffer(int size = 0);
	//Maybe return nullptr
	int* getIndexBuffer(int size = 0);

	const Vertex* getVertexAt(int i) const;
	int getIndexAt(int i) const;

	int getVertexCount() const;
	int getTriangleCount() const;

private:
	//Don't allow copy and assign from another gameobject
	GameObject(const GameObject&);
	GameObject& operator= (const GameObject& obj);
	//render data
	int* index;
	int index_size;
	Vertex* buffer;
	int buffer_size;

	void releaseVertexBuffer();
	void releaseIndexBuffer();
};

