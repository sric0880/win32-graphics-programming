#include "stdafx.h"
#include "Camera.h"
#include <cmath>
#include "OutputDebug.h"

Camera::Camera(void):
	isOrthProjection(false),
	isProjectOrthDirty(true),
	isProjectPerspDirty(true),
	isViewportDirty(true),
	isViewDirty(false),
	orthBoxHeight(10.0f),
	fieldOfView(60.0f),
	nearClippingPlane(0.3f),
	farClippingPlane(300.0f),
	viewPortHeight(0),
	viewPortWidth(0),
	widthHeightRatio(1)
{
}

Camera::~Camera(void)
{
}

void Camera::setIsOrthProjection(bool isOrth)
{
	isOrthProjection = isOrth;
}
void Camera::setOrthBoxHeight(float size)
{
	orthBoxHeight = size;
	isProjectOrthDirty = true;
}
void Camera::setFieldOfView(float size)
{
	fieldOfView = size;
	isProjectPerspDirty = true;
}
void Camera::setNearClippingPlane(float dis)
{
	nearClippingPlane = dis;
	setDirtyFlag();
}
void Camera::setFarClippingPlane(float dis)
{
	farClippingPlane = dis;
	setDirtyFlag();
}
void Camera::setViewPortWidth(int w)
{
	if (viewPortWidth != w && w != 0)
	{
		viewPortWidth = w;
		widthHeightRatio = (float)viewPortHeight / w;
		setDirtyFlag();
	}
}
void Camera::setViewPortHeight(int h)
{
	if (viewPortHeight != h && viewPortWidth != 0)
	{
		viewPortHeight = h;
		widthHeightRatio = (float)h / viewPortWidth;
		setDirtyFlag();
	}
}
const Matrix& Camera::getViewMatrix()
{
	if (isViewDirty)
	{
		view = ~transform.getTransform();
		isViewDirty = false;
#ifdef _DEBUG
	print("View Matrix: \r\n");
	view.log();
#endif
	}
	return view;
}
const Matrix& Camera::getProjectionMatrix()
{
	if (isOrthProjection)
	{
		if (isProjectOrthDirty)
		{
			//Same as Unity3D
			float width = orthBoxHeight / widthHeightRatio;
			float zrange = nearClippingPlane - farClippingPlane;
			projectOrth.m[0].x = 2 / width;
			projectOrth.m[1].y = 2 / orthBoxHeight;
			projectOrth.m[2].z = -2 / zrange;
			projectOrth.m[3].x = 0;
			projectOrth.m[3].y = 0;
			projectOrth.m[3].z = (nearClippingPlane + farClippingPlane) / zrange;
			isProjectOrthDirty = false;
#ifdef _DEBUG
		print("Orth Projection: \r\n");
		projectOrth.log();
#endif	
		}
		return projectOrth;
	}
	else
	{
		if (isProjectPerspDirty)
		{
			float t = tan(3.14159/180*fieldOfView/2);
			float zrange = nearClippingPlane - farClippingPlane;
			projectPersp.m[0].x = - widthHeightRatio/t;
			projectPersp.m[1].y = - 1/t;
			projectPersp.m[2].z = (nearClippingPlane + farClippingPlane) / zrange;
			projectPersp.m[2].w = -1;
			projectPersp.m[3].z = - 2*nearClippingPlane*farClippingPlane / zrange;
			projectPersp.m[3].w = 0;
			isProjectPerspDirty = false;
#ifdef _DEBUG
		print("Perspective Projection: \r\n");
		projectPersp.log();
#endif
		}
		return projectPersp;
	}
}

const Matrix& Camera::getViewportMatrix()
{
	if (isViewportDirty)
	{
		viewport.m[0].x = viewPortWidth*0.5;
		viewport.m[1].y = viewPortHeight*0.5;
		viewport.m[2].z = 0.5;
		viewport.m[3].x = viewPortWidth*0.5;
		viewport.m[3].y = viewPortHeight*0.5;
		viewport.m[3].z = 0.5;
		viewport.m[3].w = 1;
		isViewportDirty = false;
#ifdef _DEBUG
	print("Viewport: \r\n");
	viewport.log();
#endif
	}
	return viewport;
}

void Camera::setDirtyFlag()
{
	isProjectOrthDirty = true;
	isProjectPerspDirty = true;
	isViewportDirty = true;
}

int Camera::getViewportWidth() const
{
	return viewPortWidth;
}
int Camera::getViewportHeight() const
{
	return viewPortHeight;
}