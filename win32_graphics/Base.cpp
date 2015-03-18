#include "stdafx.h"
#include "Base.h"
#include <cmath>

float Vector::length()
{
	return sqrtf(x*x + y*y + z*z);
}
void Vector::normalize()
{
	float len = length();
	x/=len; y/=len; z/=len;
}
Vector Vector::operator+ (const Vector& vec) const
{
	return Vector ( x + vec.x, y + vec.y, z + vec.z, w + vec.w );
}
Vector Vector::operator- (const Vector& vec) const
{
	return Vector ( x - vec.x, y - vec.y, z - vec.z, w - vec.w );
}
Vector Vector::operator* (float scale) const
{
	return Vector (x*scale, y*scale, z*scale);
}
Vector Vector::dotProduct (const Vector& vec) const
{
	return Vector ( x * vec.x, y * vec.y, z * vec.z, w * vec.w );
}
Vector Vector::crossProduct(const Vector& vec) const
{
	return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

Matrix & Matrix::operator+= (const Matrix& mat)
{
	for ( int i = 0; i < 4; ++i)
	{
		m[i].x += mat.m[i].x;
		m[i].y += mat.m[i].y;
		m[i].z += mat.m[i].z;
		m[i].w += mat.m[i].w;
	}
	return *this;
}
Matrix & Matrix::operator-= (const Matrix& mat)
{
	for ( int i = 0; i < 4; ++i)
	{
		m[i].x -= mat.m[i].x;
		m[i].y -= mat.m[i].y;
		m[i].z -= mat.m[i].z;
		m[i].w -= mat.m[i].w;
	}
	return *this;
}
Matrix & Matrix::operator*= (const Matrix& mat)
{
	for ( int i = 0; i < 4; ++i)
	{
		m[i].x *= mat.m[i].x;
		m[i].y *= mat.m[i].y;
		m[i].z *= mat.m[i].z;
		m[i].w *= mat.m[i].w;
	}
	return *this;
}
Matrix & Matrix::operator*= (float x)
{
	for ( int i = 0; i < 4; ++i)
	{
		m[i].x *= x;
		m[i].y *= x;
		m[i].z *= x;
		m[i].w *= x;
	}
	return *this;
}
Matrix & Matrix::operator/= (float x)
{
	for ( int i = 0; i < 4; ++i)
	{
		m[i].x /= x;
		m[i].y /= x;
		m[i].z /= x;
		m[i].w /= x;
	}
	return *this;
}
Matrix Matrix::operator+ (const Matrix& mat) const
{
	Matrix matrix;
	for ( int i = 0; i < 4; ++i)
	{
		matrix.m[i].x = m[i].x + mat.m[i].x;
		matrix.m[i].y = m[i].y + mat.m[i].y;
		matrix.m[i].z = m[i].z + mat.m[i].z;
		matrix.m[i].w = m[i].w + mat.m[i].w;
	}
	return matrix;
}
Matrix Matrix::operator- (const Matrix& mat) const
{
	Matrix matrix;
	for ( int i = 0; i < 4; ++i)
	{
		matrix.m[i].x = m[i].x - mat.m[i].x;
		matrix.m[i].y = m[i].y - mat.m[i].y;
		matrix.m[i].z = m[i].z - mat.m[i].z;
		matrix.m[i].w = m[i].w - mat.m[i].w;
	}
	return matrix;
}
Matrix Matrix::operator* (const Matrix& mat) const
{
	Matrix matrix;
	for (int i = 0; i < 4; ++i)
	{
		matrix.m[i].x = m[0].x*mat.m[i].x + m[1].x*mat.m[i].y + m[2].x*mat.m[i].z + m[3].x*mat.m[i].w;
		matrix.m[i].y = m[0].y*mat.m[i].x + m[1].y*mat.m[i].y + m[2].y*mat.m[i].z + m[3].y*mat.m[i].w;
		matrix.m[i].z = m[0].z*mat.m[i].x + m[1].z*mat.m[i].y + m[2].z*mat.m[i].z + m[3].z*mat.m[i].w;
		matrix.m[i].w = m[0].w*mat.m[i].x + m[1].w*mat.m[i].y + m[2].w*mat.m[i].z + m[3].w*mat.m[i].w;
	}
	return matrix;
}
Vector Matrix::operator* (const Vector& vec) const
{
	Vector vector;
	vector.x = m[0].x*vec.x + m[1].x*vec.y + m[2].x*vec.z + m[3].x*vec.w;
	vector.y = m[0].y*vec.x + m[1].y*vec.y + m[2].y*vec.z + m[3].y*vec.w;
	vector.z = m[0].z*vec.x + m[1].z*vec.y + m[2].z*vec.z + m[3].z*vec.w;
	vector.w = m[0].w*vec.x + m[1].w*vec.y + m[2].w*vec.z + m[3].w*vec.w;
	vector.x /= vector.w;
	vector.y /= vector.w;
	vector.z /= vector.w;
	vector.w = 1;
	return vector;
}
Matrix Matrix::operator* (float x) const
{
	Matrix matrix;
	for ( int i = 0; i < 4; ++i)
	{
		matrix.m[i].x = m[i].x * x;
		matrix.m[i].y = m[i].y * x;
		matrix.m[i].z = m[i].z * x;
		matrix.m[i].w = m[i].w * x;
	}
	return matrix;
}
Matrix Matrix::operator/ (float x) const
{
	Matrix matrix;
	for ( int i = 0; i < 4; ++i)
	{
		matrix.m[i].x = m[i].x / x;
		matrix.m[i].y = m[i].y / x;
		matrix.m[i].z = m[i].z / x;
		matrix.m[i].w = m[i].w / x;
	}
	return matrix;
}
//Matrix Matrix::operator~ () const
//{
//	Matrix matrix;
//
//	return matrix;
//}

Matrix operator* (float x, const Matrix& mat)
{
	return mat*x;
}
Vector operator* (float x, const Vector& vert)
{
	return vert*x;
}