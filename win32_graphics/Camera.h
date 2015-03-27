#pragma once
#include "Transform.h"
enum CameraMoving{
	Forward,
	Backward,
	Left,
	Right
};

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
	bool getIsOrthProjection() { return isOrthProjection; };
	void setOrthBoxHeight(float);
	void setFieldOfView(float);
	void setNearClippingPlane(float);
	void setFarClippingPlane(float);
	void setViewPortWidth(int);
	void setViewPortHeight(int);
	void setMovingDirection(CameraMoving md);

	const Matrix& getViewMatrix();
	const Matrix& getProjectionMatrix();
	const Matrix& getViewportMatrix();
	int getViewportWidth() const;
	int getViewportHeight() const;
	float getFieldOfView() const { return fieldOfView; }
	float getHorizonFieldOfView() const;
	const Transform& getTransform() const { return transform; }
	
	float movingSpeed;
	CameraMoving movingDir;
	void moveForward(float delta)
	{
		move(delta, Vector::forward);
	}
	inline void moveBackward(float delta)
	{
		move(delta, Vector::backward);
	}
	void moveLeft(float delta)
	{
		move(delta, Vector::left);
	}
	void moveRight(float delta)
	{
		move(delta, Vector::right);
	}
private:
	Transform transform;
	bool isOrthProjection;
	float orthBoxHeight;
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

	void move(float delta, const Vector& dir);

};