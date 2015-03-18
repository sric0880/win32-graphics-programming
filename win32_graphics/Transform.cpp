#include "Transform.h"
#include <cmath>

void Transform::setTranslation(float x, float y, float z)
{
	translation.m[3].x = x;
	translation.m[3].y = y;
	translation.m[3].z = z;
	isDirty = true;
}
void Transform::setRotation(float pitch, float yaw, float roll)
{
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

const Vector& Transform::getTranslation() const
{
	return Vector(transform.m[3].x, transform.m[3].y, transform.m[3].z);
}
const Vector& Transform::getEulerAngle() const
{
	return eulerAngle;
}
const Vector& Transform::getQuaternion() const
{
	return quaternion;
}
const Vector& Transform::getScale() const
{
	return Vector(transform.m[0].x, transform.m[1].y, transform.m[2].z);
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
	transform = translation*quaternionToMatrix(quaternion)*scale;
	isDirty = false;
}

/////
// FIXME: is it for left-hand system???
//see http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
Vector Transform::quaternionToEulerAngle(const Vector& q1)
{
	Vector v;
	float test = q1.x*q1.y + q1.z*q1.w;
	if (test > 0.499) { // singularity at north pole
		v.y = 2 * atan2(q1.x,q1.w);
		v.z = 3.14159/2;
		v.x = 0;
		return;
	}
	if (test < -0.499) { // singularity at south pole
		v.y = -2 * atan2(q1.x,q1.w);
		v.z = - 3.14159/2;
		v.x = 0;
		return;
	}
    float sqx = q1.x*q1.x;
    float sqy = q1.y*q1.y;
    float sqz = q1.z*q1.z;
	v.y = atan2(2*q1.y*q1.w-2*q1.x*q1.z , 1 - 2*sqy - 2*sqz);
	v.z = asin(2*test);
	v.x = atan2(2*q1.x*q1.w-2*q1.y*q1.z , 1 - 2*sqx - 2*sqz);

	return v;
}
/////
// FIXME: is it for left-hand system???
//see http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
Vector Transform::eulerAngleToQuaternion(const Vector& e)
{
	// Assuming the angles are in radians.
    float c1 = cos(e.y/2);
    float s1 = sin(e.y/2);
    float c2 = cos(e.z/2);
    float s2 = sin(e.z/2);
    float c3 = cos(e.x/2);
    float s3 = sin(e.x/2);
    float c1c2 = c1*c2;
    float s1s2 = s1*s2;
	return Vector
		(
		c1c2*s3 + s1s2*c3,
		s1*c2*c3 + c1*s2*s3,
		c1*s2*c3 - s1*c2*s3,
		c1c2*c3 - s1s2*s3
		);
}

//////
// FIXME: is it for left-hand system??? 
//see http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
Matrix Transform::quaternionToMatrix(const Vector& q)
{
	Matrix mat;
	float sqw = q.w*q.w;
    float sqx = q.x*q.x;
    float sqy = q.y*q.y;
    float sqz = q.z*q.z;

    // invs (inverse square length) is only required if quaternion is not already normalised
    float invs = 1 / (sqx + sqy + sqz + sqw);
    mat.m[0].x = ( sqx - sqy - sqz + sqw)*invs ; // since sqw + sqx + sqy + sqz =1/invs*invs
	mat.m[1].y = (-sqx + sqy - sqz + sqw)*invs ;
	mat.m[2].z = (-sqx - sqy + sqz + sqw)*invs ;
    
    float tmp1 = q.x*q.y;
    float tmp2 = q.z*q.w;
    mat.m[0].y = 2.0 * (tmp1 + tmp2)*invs ;
    mat.m[1].x = 2.0 * (tmp1 - tmp2)*invs ;
    
    tmp1 = q.x*q.z;
    tmp2 = q.y*q.w;
    mat.m[0].z = 2.0 * (tmp1 - tmp2)*invs ;
    mat.m[2].x = 2.0 * (tmp1 + tmp2)*invs ;
    tmp1 = q.y*q.z;
    tmp2 = q.x*q.w;
	mat.m[1].z = 2.0 * (tmp1 + tmp2)*invs ;
    mat.m[2].y = 2.0 * (tmp1 - tmp2)*invs ;      
}
