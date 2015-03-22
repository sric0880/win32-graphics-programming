#include "stdafx.h"
#include "GameObject.h"
#include <cassert>
#include <iostream>

#include "OutputDebug.h"

GameObject::GameObject(void)
	:buffer(nullptr), index(nullptr),
	buffer_size(0), index_size(0)
{
}
GameObject::~GameObject(void)
{
#ifdef _DEBUG
	print("GameObjet destroy\r\n");
#endif
	releaseVertexBuffer();
	releaseIndexBuffer();
}

GameObject::GameObject(GameObject&& obj)
{
#ifdef _DEBUG
	print("GameObject copy\r\n");
#endif
	this->buffer = obj.buffer;
	this->buffer_size = obj.buffer_size;
	this->index = obj.index;
	this->index_size = obj.index_size;
	this->transform = obj.transform;
	obj.buffer = nullptr;
	obj.buffer_size = 0;
	obj.index = nullptr;
	obj.index_size = 0;
}

GameObject& GameObject::operator= (GameObject&& obj)
{
#ifdef _DEBUG
	print("GameObject assign!\r\n");
#endif
	//Not exception safe here!!! But it's ok
	releaseVertexBuffer();
	releaseIndexBuffer();
	this->buffer = obj.buffer;
	this->buffer_size = obj.buffer_size;
	this->index = obj.index;
	this->index_size = obj.index_size;
	this->transform = obj.transform;
	obj.buffer = nullptr;
	obj.buffer_size = 0;
	obj.index = nullptr;
	obj.index_size = 0;
	return *this;
}

Vertex* GameObject::getVertexBuffer(int size)
{
	if (size == 0) return buffer;
	if (buffer && buffer_size != size)
	{
		releaseVertexBuffer();
	}
	if(!buffer && size != 0)
	{
		//buffer = (Vertex*) malloc( sizeof(Vertex) * size);
		buffer = new Vertex[size]; //need to call constructor
		buffer_size = size;
	}
	return buffer;
}

int* GameObject::getIndexBuffer(int size)
{
	if (size == 0) return index;
	if (index && index_size != size)
	{
		releaseIndexBuffer();
	}
	if (!index && size != 0)
	{
		index = (int*)malloc(sizeof(int)* size);
		index_size = size;
	}
	return index;
}

const Vertex* GameObject::getVertexAt(int i) const
{
	assert(buffer!=nullptr);
	return buffer + i;
}

int GameObject::getIndexAt(int i) const
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

void GameObject::releaseVertexBuffer()
{
	//if (buffer) free(buffer);
	if (buffer) delete []buffer;
	buffer = nullptr;
	buffer_size = 0;
}
void GameObject::releaseIndexBuffer()
{
	if (index) free(index);
	index = nullptr;
	index_size = 0;
}