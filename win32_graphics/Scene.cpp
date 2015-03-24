#include "stdafx.h"
#include "Scene.h"
#include "Geometry.h"
#include "Texture2D.h"

#include "OutputDebug.h"

Scene::Scene():
	allFragments(nullptr),
	vertexBuffer(nullptr),
	vertexBufferSize(1000),
	fragmentsSize(1000),
	frameBuffer(nullptr),
	depthBuffer(nullptr),
	isDrawline(false),
	camera(new Camera())
{
	initFrameBuffer();
	initDepthBuffer();
}

Scene::~Scene()
{
	releaseFrameBuffer();
	releaseDepthBuffer();
	if (allFragments) free(allFragments);
	if (vertexBuffer) free(vertexBuffer);
}

Camera* Scene::mainCamera()
{
	return camera.get();
}

void Scene::addGameObject(GameObject&& obj)
{
	objects.emplace_back(std::forward<GameObject&&>(obj));
}
void Scene::drawScene(HDC hdc, int w, int h)
{
	camera->setViewPortWidth(w);
	camera->setViewPortHeight(h);
	clearFrameBuffer();
	clearDepthBuffer();
	for(auto& ptr_object : objects)
	{
		Matrix transform = ptr_object.transform.getTransform();
#ifdef _DEBUG
		print("Object tranform matrix: \r\n");
		transform.log();
#endif
		modelViewMatrix = camera->getViewMatrix() * transform;
#ifdef _DEBUG
		print("model view matrix: \r\n");
		modelViewMatrix.log();
#endif
		projModelViewMatrix = camera->getProjectionMatrix() * modelViewMatrix;
#ifdef _DEBUG
		print("total projmodelview matrix: \r\n");
		projModelViewMatrix.log();
#endif
		render(ptr_object);
	}
	drawPixels(hdc);
}

// draw buffer contains position, normal, color, texcoord that can put into triangles using index
void Scene::render(const GameObject& obj) //gameobject must be const
{
	int vertexCount = obj.getVertexCount();
	if (!vertexBuffer)
	{
		vertexBuffer = (Vertex*)malloc(sizeof(Vertex)* vertexCount);
		vertexBufferSize = vertexCount;
	}else if (vertexBufferSize < vertexCount)
	{
		resizeVertexBuffer(vertexCount);
	}
	for (int i = 0; i < vertexCount; ++i)
	{
		processVertex(obj.getVertexAt(i), vertexBuffer + i);
	}
	int countOfTriangles = obj.getTriangleCount();
	for (int i = 0; i < countOfTriangles; ++i)
	{
		//backface culling
		Vertex* v1 = vertexBuffer + obj.getIndexAt(3 * i);
		Vertex* v2 = vertexBuffer + obj.getIndexAt(3 * i + 1);
		Vertex* v3 = vertexBuffer + obj.getIndexAt(3 * i + 2);
		if (isBackface(*v1, *v2, *v3)) continue;

		//start clipping
		Vertex output[6]; // max count == 6 && min count == 3
		int count = clippingTriangle(v1, v2, v3, output);
		count -= 2;
		for (int j = 0; j < count; ++j)
		{
			int size = generateFragment(output[0], output[j+1], output[j+2] );
			print("fragments size: %d", size);
			processFragment(size, obj.getTexture2D());
			depthTest(size);
		}
	}
}

void Scene::processVertex(const Vertex* input, Vertex* output)
{
	// projection modelview transform
/*	print("before vertex transform: \r\n");
	input->position.log()*/;
	Vector posInCameraView = modelViewMatrix * input->position;
	output->position = camera->getProjectionMatrix() * posInCameraView;
	//print("after vertex transform : \r\n");
	//output->position.log();

	//TODO: transform normal to camera space then normalize

	//TODO: lighting per vertex

	output->color = input->color;
	output->normal = input->normal;

	if (!camera->getIsOrthProjection()){
		output->texCoord = input->texCoord  * (output->position.z);
	}
	else {
		output->texCoord = input->texCoord;
	}
}

int Scene::generateFragment(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	if (!allFragments)
	{
		allFragments = (Fragment*)malloc(sizeof(Fragment)*fragmentsSize);
	}

	// transform to view port
	Vector pos1 = camera->getViewportMatrix() * v1.position;
	Vector pos2 = camera->getViewportMatrix() * v2.position;
	Vector pos3 = camera->getViewportMatrix() * v3.position;

	//Draw line
	if (isDrawline)
	{
		int count = max( fabs(pos1.x - pos2.x), fabs(pos1.y - pos2.y));
		count += max( fabs(pos1.x - pos3.x), fabs(pos1.y - pos3.y));
		count += max( fabs(pos3.x - pos2.x), fabs(pos3.y - pos2.y));
		count += 3; // for safe
		if (count > fragmentsSize)
		{
			resizeFragments(count);
		}
		allFragments[0].x = pos1.x;
		allFragments[0].y = pos1.y;
		allFragments[1].x = pos2.x;
		allFragments[1].y = pos2.y;
		allFragments[2].x = pos3.x;
		allFragments[2].y = pos3.y;

		int c = 3;
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[1]);
		c+= drawLineBresenham(allFragments+c, &allFragments[1], &allFragments[2]);
		c+= drawLineBresenham(allFragments+c, &allFragments[0], &allFragments[2]);
		//set all the line color to black
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
		//Rasterization
		FillData data;
		scanTriangle(&data, pos1, pos2, pos3);
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

		//Interpolation
		Matrix mat;
		mat.m[0].x = pos1.x;
		mat.m[1].x = pos2.x;
		mat.m[2].x = pos3.x;
		mat.m[0].y = pos1.y;
		mat.m[1].y = pos2.y;
		mat.m[2].y = pos3.y;
		mat.m[0].z = 1;
		mat.m[1].z = 1;
		mat.m[2].z = 1;
		Matrix interpolationMatrix = ~mat;
		for(int i = 0; i < c; ++i)
		{
			Vector interp = interpolationMatrix * Vector(allFragments[i].x, allFragments[i].y, 1);
			//Color interpolation
			allFragments[i].color = v1.color * interp.x + v2.color * interp.y + v3.color * interp.z;
			clerp(0, 1, allFragments[i].color);
			
			//Normal interpolation
			allFragments[i].normal = v1.normal * interp.x + v2.normal * interp.y + v3.normal * interp.z;
			
			//Depth interpolation
			//z is not linear but 1/z is linear!
			allFragments[i].depth = pos1.z * interp.x + pos2.z * interp.y + pos3.z * interp.z;

			//Texture Coord interpolation
			//allFragments[i].texCoord = (v1.texCoord* interp.x + v2.texCoord* interp.y + v3.texCoord* interp.z);
			allFragments[i].texCoord = (v1.texCoord* interp.x + v2.texCoord * interp.y + v3.texCoord * interp.z) * (0.5 / (allFragments[i].depth-0.5f));
			clerp(0, 1, allFragments[i].texCoord);
			
		}
		return c;
	}
}

void Scene::processFragment(int size, const Texture2D* tex)
{
	for(int i = 0; i < size; ++i)
	{
		Fragment& frag = allFragments[i];
		//Texture mapping
		if (!tex->isEmpty())
		{
			frag.color = tex->sampleTexture2D(frag.texCoord.x, frag.texCoord.y);
		}
			
		//TODO: Lighting per pixel

	}
}

void Scene::depthTest(int size)
{
	int w = camera->getViewportWidth();
	int h = camera->getViewportHeight();
	for (int i = 0; i < size; ++i)
	{
		auto& frame = allFragments[i];
		int index_ = bufferIndex(frame.x, frame.y, w, h);
		int index = 3*index_;
		if (isDrawline) //not need test depth
		{
			//update to frame buffer
			frameBuffer[index] = allFragments[i].color.x * 0xff;
			frameBuffer[index + 1] = allFragments[i].color.y * 0xff;
			frameBuffer[index + 2] = allFragments[i].color.z * 0xff;
		}
		else if (depthBuffer[index_] > allFragments[i].depth)
		{
			depthBuffer[index_] = allFragments[i].depth;

			//update to frame buffer
			frameBuffer[index] = allFragments[i].color.x * 0xff;
			frameBuffer[index + 1] = allFragments[i].color.y * 0xff;
			frameBuffer[index + 2] = allFragments[i].color.z * 0xff;
		}
	}
}
void Scene::drawPixels(HDC hdc)
{
	int viewportWidth = camera->getViewportWidth();
	int viewportHeight = camera->getViewportHeight();
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, viewportWidth, viewportHeight);
	SelectObject(mdc, bitmap);

	//draw pixels
	int pixelsCount = viewportWidth * viewportHeight;
	for (int i = 0; i < viewportWidth; ++i)
	{
		for (int j = 0; j < viewportHeight; ++j)
		{
			int index =3* windowCoordToBufferIndex(i, j, viewportWidth, viewportHeight);
			SetPixel(mdc, i, j, RGB(frameBuffer[index], frameBuffer[index+1], frameBuffer[index+2]));
		}
	}

	//swap to hdc
	BitBlt(hdc, 0, 0, viewportWidth, viewportHeight, mdc, 0, 0, SRCCOPY);
	//release
	DeleteDC(mdc);
	DeleteObject(bitmap);
}

void Scene::resizeFragments(int size)
{
	allFragments = (Fragment*)realloc(allFragments, sizeof(Fragment)*size);
	fragmentsSize = size;
}
void Scene::resizeVertexBuffer(int size)
{
	vertexBuffer = (Vertex*)realloc(vertexBuffer, sizeof(Vertex)* size);
	vertexBufferSize = size;
}

const int framebufferSize = 1680*1050*3;
const int depthbufferSize = 1680*1050*sizeof(float);

void Scene::initFrameBuffer()
{
	if (!frameBuffer)
		frameBuffer = (UINT8*) malloc( framebufferSize );
}
void Scene::initDepthBuffer()
{
	if (!depthBuffer)
		depthBuffer = (float*) malloc(depthbufferSize);
}
void Scene::clearFrameBuffer()
{
	if(frameBuffer)
		memset(frameBuffer, 0xff, framebufferSize);
}
void Scene::clearDepthBuffer()
{
	if (depthBuffer)
	{
		int c = depthbufferSize / 4;
		for (int i = 0; i < c; ++i) depthBuffer[i] = 1.0f;
	}
}
void Scene::releaseFrameBuffer()
{
	if(frameBuffer)
		free(frameBuffer);
	frameBuffer = nullptr;
}
void Scene::releaseDepthBuffer()
{
	if(depthBuffer)
		free(depthBuffer);
	depthBuffer = nullptr;
}

inline int Scene::bufferIndex(int x, int y, int viewportWidth, int viewportHeight)
{
	return viewportWidth * y + x ;
}

inline int Scene::windowCoordToBufferIndex(int x, int y, int viewportWidth, int viewportHeight)
{
	return viewportWidth* (viewportHeight - y) + x;
}