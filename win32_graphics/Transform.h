#pragma once
#include "base.h"
class Transform
{
public:
	Transform():isDirty(false){}
	~Transform(){}
	void setTranslation(float x, float y, float z);
	void setRotation(float pitch, float yaw, float roll);
	void setScale(float x, float y, float z);

	Vector getTranslation() const;
	const Vector& getEulerAngle() const;
	const Vector& getQuaternion() const;
	Vector getScale() const;
	const Matrix& getTransform() const;

	static Vector quaternionToEulerAngle(const Vector& q1);
	static Vector eulerAngleToQuaternion(const Vector& e);
	static Matrix quaternionToMatrix(const Vector& q);
private:
	Matrix translation;
	Vector quaternion;
	Vector eulerAngle;
	Matrix scale;
	Matrix transform;
	bool isDirty;

	void updateTransform();
};