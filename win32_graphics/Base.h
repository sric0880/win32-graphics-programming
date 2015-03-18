#pragma once
struct Matrix;
struct Vector
{
	float x;
	float y;
	float z;
	float w;

	Vector():x(0),y(0),z(0),w(1){}
	Vector(float _x, float _y, float _z, float _w = 1):x(_x), y(_y), z(_z), w(_w){}
	float length();
	void normalize();
	Vector dotProduct (const Vector& vec) const;
	Vector crossProduct(const Vector& vec) const;
	Vector operator+ (const Vector& vec) const;
	Vector operator- (const Vector& vec) const;
	Vector operator* (float scale) const;
};

struct Matrix
{
	Vector m[4];
	Matrix() // identity matrix
	{
		identityMatrix(*this);
	}
	
	Matrix & operator+= (const Matrix& mat);
	Matrix & operator-= (const Matrix& mat);
	Matrix & operator*= (const Matrix& mat);
	Matrix & operator*= (float x);
	Matrix & operator/= (float x);
	Matrix operator+ (const Matrix& mat) const;
	Matrix operator- (const Matrix& mat) const;
	Matrix operator* (const Matrix& mat) const;
	Vector operator* (const Vector& vec) const;
	Matrix operator* (float x) const;
	Matrix operator/ (float x) const;
	//Matrix operator~ () const;

	static void identityMatrix(Matrix& m)
	{
		m.m[0].x = 1; m.m[0].y = 0; m.m[0].z = 0; m.m[0].w = 0;
		m.m[1].x = 0; m.m[1].y = 1; m.m[1].z = 0; m.m[1].w = 0;
		m.m[0].x = 0; m.m[0].y = 0; m.m[0].z = 1; m.m[0].w = 0;
		m.m[0].x = 0; m.m[0].y = 0; m.m[0].z = 0; m.m[0].w = 1;
	}
};

Matrix operator* (float x, const Matrix& mat);
Vector operator* (float x, const Vector& vert);
//Matrix operator* (const Vector& vec, const Matrix& mat); // undefined

struct Position
{
	float x;
	float y;
	float z;
};

struct Color
{
	float r;
	float g;
	float b;
	float alpha;
};

struct TexCoord
{
	float u;
	float v;
};

struct Vertex
{
	Position position;
	Color color;
	TexCoord texCoord;
	Vector normal;
};

struct Fragment
{
	float x; // x in viewport
	float y; // y in viewport
	Color color;
	Vector normal;
	TexCoord texCoord;
	float depth;
};

//struct Triangle
//{
//	Vertex v[3];
//};

