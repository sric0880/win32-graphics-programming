#pragma once
#include "Transform.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	
	void setTranslation(float x, float y, float z)
	{
		transform.setTranslation(x, y, z);
		isViewDirty = true;
	}

	void setRotation(float pitch, float yaw, float roll)
	{
		transform.setRotation(pitch, yaw, roll);
		isViewDirty = true;
	}

	//camera cannot scale!!
	
	void setIsOrthProjection(bool);
	void setOrthBoxSize(float);
	void setFieldOfView(float);
	void setNearClippingPlane(float);
	void setFarClippingPlane(float);
	void setViewPortWidth(int);
	void setViewPortHeight(int);
	const Matrix& getViewMatrix();
	const Matrix& getProjectionMatrix();
	const Matrix& getViewportMatrix();
	
private:
	Transform transform;
	bool isOrthProjection;
	float orthBoxSize;
	float fieldOfView;
	float nearClippingPlane;
	float farClippingPlane;
	int viewPortWidth;
	int viewPortHeight;
	Matrix view;
	Matrix projectOrth;
	Matrix projectPersp;
	Matrix viewport;

	bool isViewDirty;
	bool isProjectOrthDirty;
	bool isProjectPerspDirty;
	bool isViewportDirty;

	float widthHeightRatio;
	void setDirtyFlag();

};