#pragma once
#include "Base.h"
#include <vector>
struct Line
{
	int left;
	int right;
	Line(int l, int r): left(l), right(r){}
};
struct FillData
{
	int ymin;
	int fragmentsCount;
	std::vector<Line> lines;
	FillData(): ymin(0), fragmentsCount(0) {}
};

int drawLineBresenham(Fragment* buffer, const Fragment* start, const Fragment* end);
void scanTriangle(FillData* data, const Vector& p1, const Vector& p2, const Vector& p3);