#include "stdafx.h"
#include "Transform.h"
#include <cmath>
#include "OutputDebug.h"

void Transform::setTranslation(float x, float y, float z)
{
	translation.m[3].x = x;
	translation.m[3].y = y;
	translation.m[3].z = z;
	isDirty = true;
}
void Transform::setRotation(float pitch, float yaw, float roll)
{
	//In ZXY order (same as Unity3D)
	eulerAngle.x = pitch;	
	eulerAngle.y = yaw;
	eulerAngle.z = roll;
	quaternion = eulerAngleToQuaternion(eulerAngle);
	isDirty = true;
}

void Transform::setScale(float x, float y, float z)
{
	scale.m[0].x = x;
	scale.m[1].y = y;
	scale.m[2].z = z;
	isDirty = true;
}

Vector Transform::getTranslation() const
{
	return Vector(translation.m[3].x, translation.m[3].y, translation.m[3].z);
}
const Matrix& Transform::getTranslationMatrix() const
{
	return translation;
}
const Vector& Transform::getEulerAngle() const
{
	return eulerAngle;
}
Matrix Transform::getRotationMatrix() const
{
	return quaternionToMatrix(quaternion);
}
const Vector& Transform::getQuaternion() const
{
	return quaternion;
}
Vector Transform::getScale() const
{
	return Vector(scale.m[0].x, scale.m[1].y, scale.m[2].z);
}
const Matrix& Transform::getScaleMatrix() const
{
	return scale;
}
const Matrix& Transform::getTransform() const
{
	if (isDirty)
	{
		Transform* t = const_cast<Transform*>(this);
		t->updateTransform();
	}
	return transform;
}
void Transform::updateTransform()
{
	Matrix::identityMatrix(transform);
#ifdef _DEBUG
	print("Rotation: \r\n");
	quaternion.log();
	quaternionToEulerAngle(quaternion).log();
	//print("Quaternion Matrix: \r\n");
	//Matrix quat = quaternionToMatrix(quaternion);
	//quat.log();
	//Matrix mat = eulerAngleToMatrix(eulerAngle);
	//print("Euler Matrix: \r\n");
	//mat.log();
	print("Translation: \r\n");
	translation.log();
#endif
	transform = translation*quaternionToMatrix(quaternion)*scale;
	isDirty = false;
}

/////
//see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Vector Transform::quaternionToEulerAngle(const Vector& q1)
{
	Vector v;
    float sqx = q1.x*q1.x;
    float sqy = q1.y*q1.y;
    float sqz = q1.z*q1.z;
	//In YXZ order
	//v.y = atan2(2*q1.w*q1.x+2*q1.y*q1.z , 1 - 2*sqx - 2*sqy);
	//v.x = asin(2*(q1.w*q1.y - q1.z*q1.x));
	//v.z = atan2(2*q1.w*q1.z+2*q1.x*q1.y , 1 - 2*sqy - 2*sqz);
	
	v.y = atan2(2*q1.w*q1.z-2*q1.x*q1.y , 1 - 2*sqx - 2*sqz);
	v.x = asin(2*(q1.w*q1.x + q1.y*q1.z));
	v.z = atan2(2*q1.w*q1.y-2*q1.x*q1.z , 1 - 2*sqx - 2*sqy);

	v.x = toDegree(v.x);
	v.y = toDegree(v.y);
	v.z = toDegree(v.z);

	return v;
}
///// different order to different mathod
//see http://gamedev.stackexchange.com/questions/13436/glm-euler-angles-to-quaternion
//also see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Vector Transform::eulerAngleToQuaternion(const Vector& e)
{
	float x = toRadian(e.x);
	float y = toRadian(e.y);
	float z = toRadian(e.z);
	// Assuming the angles are in radians.
    /*float c1 = cos(x/2);
    float s1 = sin(x/2);
    float c2 = cos(y/2);
    float s2 = sin(y/2);
    float c3 = cos(z/2);
    float s3 = sin(z/2);*/

	//In YXZ order
	float c1 = cos(y/2);
    float s1 = sin(y/2);
    float c2 = cos(x/2);
    float s2 = sin(x/2);
    float c3 = cos(z/2);
    float s3 = sin(z/2);

    float c1c2 = c1*c2;
    float s1s2 = s1*s2;
	/*return Vector
		(
		s1*c2*c3 - c1*s2*s3,
		c1*s2*c3 + s1*c2*s3,
		c1c2*s3 - s1s2*c3,
		c1c2*c3 + s1s2*s3
		);*/
	return Vector
		(
		c1*s2*c3 - s1*c2*s3,
		s1s2*c3 + c1c2*s3,
		s1*c2*c3 + c1*s2*s3,
		c1c2*c3 - s1s2*s3
		);
}

//////
// inhomogeneous expression because q is unit quaternion
//see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Matrix Transform::quaternionToMatrix(const Vector& q)
{
	Matrix mat;
	float sqw = q.w*q.w;
    float sqx = q.x*q.x;
    float sqy = q.y*q.y;
    float sqz = q.z*q.z;

    mat.m[0].x = 1- 2*sqy - 2*sqz;
	mat.m[1].y = 1- 2*sqx - 2*sqz;
	mat.m[2].z = 1- 2*sqx - 2*sqy;
    
    float tmp1 = q.x*q.y;
    float tmp2 = q.z*q.w;
    mat.m[0].y = 2.0f * (tmp1 + tmp2);
    mat.m[1].x = 2.0f * (tmp1 - tmp2) ;
    
    tmp1 = q.x*q.z;
    tmp2 = q.y*q.w;
    mat.m[0].z = 2.0f * (tmp1 - tmp2);
    mat.m[2].x = 2.0f * (tmp1 + tmp2) ;
    tmp1 = q.y*q.z;
    tmp2 = q.x*q.w;
	mat.m[1].z = 2.0f * (tmp1 + tmp2);
    mat.m[2].y = 2.0f * (tmp1 - tmp2);

	return mat;
}

Matrix Transform::eulerAngleToMatrix(const Vector& e)
{
	Matrix mat;
	float x = toRadian(e.x);
	float y = toRadian(e.y);
	float z = toRadian(e.z);

	float c1 = cos(y);
	float s1 = sin(y);
	float c2 = cos(x);
	float s2 = sin(x);
	float c3 = cos(z);
	float s3 = sin(z);

	// YXZ order
	mat.m[0].x = c1*c3 - s1*s2*s3;
	mat.m[1].x = -s1*c2;
	mat.m[2].x = s1*s2*c3 + c1*s3;
	mat.m[0].y = c1*s2*s3 + s1*c3;
	mat.m[1].y = c1*c2;
	mat.m[2].y = -c1*s2*c3 + s1*s3;
	mat.m[0].z = -c2*s3;
	mat.m[1].z = s2;
	mat.m[2].z = c2*c3;

/*	mat.m[0].x = c1*c3 + s1*s2*s3;
	mat.m[1].x = s1*s2*c3 - c1*s3;
	mat.m[2].x = s1*c2;
	mat.m[0].y = c2*s3;
	mat.m[1].y = c2*c3;
	mat.m[2].y = -s2;
	mat.m[0].z = c1*s2*s3 - s1*c3;
	mat.m[1].z = c1*s2*c3 + s1*s3;
	mat.m[2].z = c1*c2*/;

	return mat;
}