#pragma once
struct Matrix;
struct Vector
{
	float x;
	float y;
	float z;
	float w;

	Vector(){}
	Vector(float _x, float _y, float _z, float _w = 1):x(_x), y(_y), z(_z), w(_w){}
	float length();
	void normalize();
	Vector dotProduct (const Vector& vec);
	Vector crossProduct(const Vector& vec);
	Vector operator+ (const Vector& vec);
	Vector operator- (const Vector& vec);
	//TODO: reverse a verctor
};

struct Matrix
{
	Vector m[4];

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
	//TODO: reverse a matrix
};

Matrix operator* (float x, const Matrix& mat);
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

struct Camera
{
	Position pos;
	Vector up;
	Vector orientation;
};

//struct Triangle
//{
//	Vertex v[3];
//};

