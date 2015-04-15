#pragma once
//struct Matrix;
struct Vector
{
	float x;
	float y;
	float z;
	float w;

	Vector():x(0),y(0),z(0),w(1){}
	Vector(float _x, float _y, float _z, float _w = 1):x(_x), y(_y), z(_z), w(_w){}
	float length() const;
	void normalize();
	float dotProduct (const Vector& vec) const;
	Vector crossProduct(const Vector& vec) const;
	Vector operator+ (const Vector& vec) const;
	Vector operator- (const Vector& vec) const;
	Vector operator* (float scale) const;
	const Vector& operator*= (const Vector& vec);
	const Vector& operator+= (const Vector& vec);
	Vector& operator*= (float scale);
	Vector& operator/= (float scale);

	static Vector Max(const Vector& v1, const Vector& v2);
	const static Vector zero;
	const static Vector forward;
	const static Vector backward;
	const static Vector left;
	const static Vector right;
	const static Vector up;
	const static Vector down;

	void log() const;
	void toInt();
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
	Matrix operator~ () const;
	void log() const;

	static void identityMatrix(Matrix& m)
	{
		m.m[0].x = 1; m.m[0].y = 0; m.m[0].z = 0; m.m[0].w = 0;
		m.m[1].x = 0; m.m[1].y = 1; m.m[1].z = 0; m.m[1].w = 0;
		m.m[2].x = 0; m.m[2].y = 0; m.m[2].z = 1; m.m[2].w = 0;
		m.m[3].x = 0; m.m[3].y = 0; m.m[3].z = 0; m.m[3].w = 1;
	}
	static void Tranpose(Matrix& mat);
};

Matrix operator* (float x, const Matrix& mat);
Vector operator* (float x, const Vector& vert);
//Matrix operator* (const Vector& vec, const Matrix& mat); // undefined

struct Vertex
{
	Vector position;
	Vector color;
	Vector texCoord;
	Vector normal;
	Vector eye; //point to eye(camera) in camera space.
};

struct Fragment
{
	int x; // x in viewport
	int y; // y in viewport
	Vector color;
	Vector normal;
	Vector texCoord;
	Vector eye; //used for per pixel lighting
	float depth;
};