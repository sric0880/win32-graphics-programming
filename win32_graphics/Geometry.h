#pragma once
#include "Base.h"
#include <vector>
struct Line
{
	int left;
	int right;
	int y;
	Line(int l, int r, int y): left(l), right(r), y(y){}
};
struct FillData
{
	int fragmentsCount;
	std::vector<Line> lines;
	FillData(): fragmentsCount(0) {}
};

// clip triangle in clipping space
// parameter: 
// in -- three vertexs
// out -- output vertexs 
// return output vertexs count
int clippingTriangle(const Vertex* v1, const Vertex* v2, const Vertex* v3, Vertex* out);

// back face culling
bool isBackface(const Vertex* v1, const Vertex* v2, const Vertex* v3);

// draw line using bresenham algorithm
int drawLineBresenham(Fragment* buffer, const Fragment* start, const Fragment* end);

// scan line in a triangle
void scanTriangle(FillData* data, const Vector& p1, const Vector& p2, const Vector& p3);

inline float clerp(float min, float max, float value)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline void clerp(float min, float max, Vector& vector)
{
	vector.x = clerp(min, max, vector.x);
	vector.y = clerp(min, max, vector.y);
	vector.z = clerp(min, max, vector.z);
}