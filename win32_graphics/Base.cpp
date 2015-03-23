#include "stdafx.h"
#include "Base.h"
#include <cmath>
#include <cassert>
#include "OutputDebug.h"

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
	return Vector ( x + vec.x, y + vec.y, z + vec.z);
}
Vector Vector::operator- (const Vector& vec) const
{
	return Vector ( x - vec.x, y - vec.y, z - vec.z);
}
Vector Vector::operator* (float scale) const
{
	return Vector (x*scale, y*scale, z*scale);
}
Vector Vector::dotProduct (const Vector& vec) const
{
	return Vector ( x * vec.x, y * vec.y, z * vec.z);
}
Vector Vector::crossProduct(const Vector& vec) const
{
	return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}
void Vector::log() const
{
	print("x: %.3f, y: %.3f, z: %.3f, w: %.3f\r\n", x, y, z, w);
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

// get the inverse of matrix
//see http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
Matrix Matrix::operator~() const
{
	Matrix ret;
	float mat[16];
	for (int i =0; i < 4; ++i)
	{
		mat[i] = m[i].x;
		mat[4+i] = m[i].y;
		mat[8+i] = m[i].z;
		mat[12+i] = m[i].w;
	}
    float inv[16], det;
    int i;

    inv[0] = mat[5]  * mat[10] * mat[15] - 
             mat[5]  * mat[11] * mat[14] - 
             mat[9]  * mat[6]  * mat[15] + 
             mat[9]  * mat[7]  * mat[14] +
             mat[13] * mat[6]  * mat[11] - 
             mat[13] * mat[7]  * mat[10];

    inv[4] = -mat[4]  * mat[10] * mat[15] + 
              mat[4]  * mat[11] * mat[14] + 
              mat[8]  * mat[6]  * mat[15] - 
              mat[8]  * mat[7]  * mat[14] - 
              mat[12] * mat[6]  * mat[11] + 
              mat[12] * mat[7]  * mat[10];

    inv[8] = mat[4]  * mat[9] * mat[15] - 
             mat[4]  * mat[11] * mat[13] - 
             mat[8]  * mat[5] * mat[15] + 
             mat[8]  * mat[7] * mat[13] + 
             mat[12] * mat[5] * mat[11] - 
             mat[12] * mat[7] * mat[9];

    inv[12] = -mat[4]  * mat[9] * mat[14] + 
               mat[4]  * mat[10] * mat[13] +
               mat[8]  * mat[5] * mat[14] - 
               mat[8]  * mat[6] * mat[13] - 
               mat[12] * mat[5] * mat[10] + 
               mat[12] * mat[6] * mat[9];

    inv[1] = -mat[1]  * mat[10] * mat[15] + 
              mat[1]  * mat[11] * mat[14] + 
              mat[9]  * mat[2] * mat[15] - 
              mat[9]  * mat[3] * mat[14] - 
              mat[13] * mat[2] * mat[11] + 
              mat[13] * mat[3] * mat[10];

    inv[5] = mat[0]  * mat[10] * mat[15] - 
             mat[0]  * mat[11] * mat[14] - 
             mat[8]  * mat[2] * mat[15] + 
             mat[8]  * mat[3] * mat[14] + 
             mat[12] * mat[2] * mat[11] - 
             mat[12] * mat[3] * mat[10];

    inv[9] = -mat[0]  * mat[9] * mat[15] + 
              mat[0]  * mat[11] * mat[13] + 
              mat[8]  * mat[1] * mat[15] - 
              mat[8]  * mat[3] * mat[13] - 
              mat[12] * mat[1] * mat[11] + 
              mat[12] * mat[3] * mat[9];

    inv[13] = mat[0]  * mat[9] * mat[14] - 
              mat[0]  * mat[10] * mat[13] - 
              mat[8]  * mat[1] * mat[14] + 
              mat[8]  * mat[2] * mat[13] + 
              mat[12] * mat[1] * mat[10] - 
              mat[12] * mat[2] * mat[9];

    inv[2] = mat[1]  * mat[6] * mat[15] - 
             mat[1]  * mat[7] * mat[14] - 
             mat[5]  * mat[2] * mat[15] + 
             mat[5]  * mat[3] * mat[14] + 
             mat[13] * mat[2] * mat[7] - 
             mat[13] * mat[3] * mat[6];

    inv[6] = -mat[0]  * mat[6] * mat[15] + 
              mat[0]  * mat[7] * mat[14] + 
              mat[4]  * mat[2] * mat[15] - 
              mat[4]  * mat[3] * mat[14] - 
              mat[12] * mat[2] * mat[7] + 
              mat[12] * mat[3] * mat[6];

    inv[10] = mat[0]  * mat[5] * mat[15] - 
              mat[0]  * mat[7] * mat[13] - 
              mat[4]  * mat[1] * mat[15] + 
              mat[4]  * mat[3] * mat[13] + 
              mat[12] * mat[1] * mat[7] - 
              mat[12] * mat[3] * mat[5];

    inv[14] = -mat[0]  * mat[5] * mat[14] + 
               mat[0]  * mat[6] * mat[13] + 
               mat[4]  * mat[1] * mat[14] - 
               mat[4]  * mat[2] * mat[13] - 
               mat[12] * mat[1] * mat[6] + 
               mat[12] * mat[2] * mat[5];

    inv[3] = -mat[1] * mat[6] * mat[11] + 
              mat[1] * mat[7] * mat[10] + 
              mat[5] * mat[2] * mat[11] - 
              mat[5] * mat[3] * mat[10] - 
              mat[9] * mat[2] * mat[7] + 
              mat[9] * mat[3] * mat[6];

    inv[7] = mat[0] * mat[6] * mat[11] - 
             mat[0] * mat[7] * mat[10] - 
             mat[4] * mat[2] * mat[11] + 
             mat[4] * mat[3] * mat[10] + 
             mat[8] * mat[2] * mat[7] - 
             mat[8] * mat[3] * mat[6];

    inv[11] = -mat[0] * mat[5] * mat[11] + 
               mat[0] * mat[7] * mat[9] + 
               mat[4] * mat[1] * mat[11] - 
               mat[4] * mat[3] * mat[9] - 
               mat[8] * mat[1] * mat[7] + 
               mat[8] * mat[3] * mat[5];

    inv[15] = mat[0] * mat[5] * mat[10] - 
              mat[0] * mat[6] * mat[9] - 
              mat[4] * mat[1] * mat[10] + 
              mat[4] * mat[2] * mat[9] + 
              mat[8] * mat[1] * mat[6] - 
              mat[8] * mat[2] * mat[5];

    det = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];
	
	assert(det!=0);

    det = 1.0 / det;

    for (i = 0; i < 4; ++i)
	{
		ret.m[i].x = inv[i]*det;
		ret.m[i].y = inv[4+i]*det;
		ret.m[i].z = inv[8+i]*det;
		ret.m[i].w = inv[12+i]*det;
	}

    return ret;
}

void Matrix::log() const
{
	print("%.3f %.3f %.3f %.3f \r\n%.3f %.3f %.3f %.3f\r\n%.3f %.3f %.3f %.3f\r\n%.3f %.3f %.3f %.3f\r\n", 
		m[0].x, m[1].x, m[2].x, m[3].x,
		m[0].y, m[1].y, m[2].y, m[3].y,
		m[0].z, m[1].z, m[2].z, m[3].z,
		m[0].w, m[1].w, m[2].w, m[3].w);
}

Matrix operator* (float x, const Matrix& mat)
{
	return mat*x;
}
Vector operator* (float x, const Vector& vert)
{
	return vert*x;
}