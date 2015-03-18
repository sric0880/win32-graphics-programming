#include "stdafx.h"
#include "Camera.h"
#include <cmath>

Camera::Camera(void):
	isOrthProjection(false),
	isProjectOrthDirty(true),
	isProjectPerspDirty(true),
	isViewDirty(false),
	orthBoxSize(10.0f),
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
void Camera::setOrthBoxSize(float size)
{
	orthBoxSize = size;
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
	if (viewPortWidth != w)
	{
		viewPortWidth = w;
		widthHeightRatio = (float)viewPortHeight / w;
		setDirtyFlag();
	}
}
void Camera::setViewPortHeight(int h)
{
	if (viewPortHeight != h)
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
		auto translation = transform.getTranslation();
		auto eulerAngle = transform.getEulerAngle();
		//view matrix can move camera to world origin and rotate to match origin axes
		Transform temp;
		temp.setTranslation(-translation.x, -translation.y, -translation.z);
		temp.setRotation(-eulerAngle.x, -eulerAngle.y, -eulerAngle.z);
		view = temp.getTransform();
		isViewDirty = false;
	}
	return view;
}
const Matrix& Camera::getProjectionMatrix()
{
	if (isOrthProjection)
	{
		if (isProjectOrthDirty)
		{
			//recalculate
			//float xmin, ymin, xmax, ymax;
			float height = orthBoxSize * widthHeightRatio;
		/*	xmin = -orthBoxSize/2;
			ymin = -height/2;
			xmax = -xmin;
			ymax = -ymin;*/
			projectOrth.m[0].x = 2 / orthBoxSize;
			projectOrth.m[1].y = 2 / height;
			projectOrth.m[2].z = -2 / (nearClippingPlane - farClippingPlane);
			projectOrth.m[3].x = 0;
			projectOrth.m[3].y = 0;
			projectOrth.m[3].z = (nearClippingPlane + farClippingPlane) / (nearClippingPlane - farClippingPlane);
			isProjectOrthDirty = false;
		}
		return projectOrth;
	}
	else
	{
		if (isProjectPerspDirty)
		{
			//投影参考点位于远点 观察平面位于近剪裁平面
			/*float height = 2*nearClippingPlane*tan(3.14159/360*fieldOfView/2);
			float width = height / widthHeightRatio;*/
			float t = tan(3.14159/360*fieldOfView/2);
			projectPersp.m[0].x = -widthHeightRatio/t;
			projectPersp.m[1].y = -1/t;
			projectPersp.m[2].z = (nearClippingPlane + farClippingPlane) / (nearClippingPlane - farClippingPlane);
			projectPersp.m[2].w = -1;
			projectPersp.m[3].z = 2*nearClippingPlane*farClippingPlane / (farClippingPlane - nearClippingPlane);
			projectPersp.m[3].w = 0;
			isProjectPerspDirty = false;
		}
		return projectPersp;
	}
}

void Camera::setDirtyFlag()
{
	isProjectOrthDirty = true;
	isProjectPerspDirty = true;
}