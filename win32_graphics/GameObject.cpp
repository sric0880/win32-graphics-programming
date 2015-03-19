#include "stdafx.h"
#include "GameObject.h"
#include <cassert>

GameObject::GameObject(void)
	:buffer(nullptr), index(nullptr),
	buffer_size(0), index_size(0)
{
}
GameObject::~GameObject(void)
{
	if (buffer) free(buffer);
	if (index) free(index);
	buffer = nullptr;
	index = nullptr;
}

Vertex* GameObject::getVertexBuffer(UINT32 size)
{
	if (size == 0) return buffer;
	if (buffer && buffer_size != size)
	{
		free(buffer);
		buffer = nullptr;
	}
	if(!buffer && size != 0)
	{
		buffer = (Vertex*) malloc( sizeof(Vertex) * size);
		buffer_size = size;
	}
	return buffer;
}

int* GameObject::getIndexBuffer(UINT32 size)
{
	if (size == 0) return index;
	if (index && index_size != size)
	{
		free(index);
		index = nullptr;
	}
	if (!index && size != 0)
	{
		index = (int*)malloc(sizeof(int)* size);
		index_size = size;
	}
	return index;
}

Vertex* GameObject::getVertexAt(int i)
{
	assert(buffer!=nullptr);
	return buffer + i;
}

int GameObject::getIndexAt(int i)
{
	assert(index != nullptr);
	return index[i];
}

int GameObject::getVertexCount() const
{
	return buffer_size;
}
int GameObject::getTriangleCount() const
{
	return index_size / 3;
}