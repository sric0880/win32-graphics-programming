#pragma once
#include "base.h"
#define Pi 3.14159
#define toRadian(_x) (_x*Pi/180.0f)
#define toDegree(_x) (_x*180.0f/Pi)
class Transform
{
public:
	Transform():isDirty(false){}
	~Transform(){}
	void setTranslation(float x, float y, float z);
	void setRotation(float pitch, float yaw, float roll);
	void setScale(float x, float y, float z);

	Vector getTranslation() const;
	const Matrix& getTranslationMatrix() const;
	const Vector& getEulerAngle() const;
	Matrix getRotationMatrix() const;
	const Vector& getQuaternion() const;
	Vector getScale() const;
	const Matrix& getScaleMatrix() const;
	const Matrix& getTransform() const;

	static Vector quaternionToEulerAngle(const Vector& q1);
	static Vector eulerAngleToQuaternion(const Vector& e);
	static Matrix quaternionToMatrix(const Vector& q);
	static Matrix eulerAngleToMatrix(const Vector& eulerAngle);
private:
	Matrix translation;
	Vector quaternion;
	Vector eulerAngle;
	Matrix scale;
	Matrix transform;
	bool isDirty;

	void updateTransform();
};