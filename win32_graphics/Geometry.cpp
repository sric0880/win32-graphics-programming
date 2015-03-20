#include "stdafx.h"
#include "Geometry.h"
#include <cmath>

///////
//Draw line agorithms (not contain start and end)
int drawLineBresenham(Fragment* buffer, const Fragment* start, const Fragment* end)
{
	int start_x = start->x;
	int start_y = start->y;
	int end_x = end->x;
	int end_y = end->y;
	if (start->x > end->x){
		start_x = end->x;
		end_x = start->x;
		start_y = end->y;
		end_y = start->y;
	}
	int dx = end_x - start_x;
	int dy = end_y - start_y;
	if(dx == 0 && dy ==0) return 0;
	bool isRevert = false;
	if (dx == 0 || fabs( ((float)dy)/dx ) > 1 )
	{
		//swap x and y axes
		int temp = start_x;
		start_x = start_y;
		start_y = temp;
		temp = end_x;
		end_x = end_y;
		end_y = temp;
		isRevert = true;
	}

	int steps = 0;	
	steps = dx - 1;
	int sy = 1;
	if (dy < 0) {
		sy = -1;
		dy = -dy;
	}
	if (steps == 0) return 0;
	buffer[0].x = start_x+1;
	int e = 2*dy - dx;
	int diff = 0;
	if(e < 0) buffer[0].y = start_y;
	else { buffer[0].y = start_y + sy; diff = 1; }
	for (int i = 1; i < steps; ++i)
	{
		buffer[i].x = buffer[i-1].x + 1;
		e+=(2*dy - 2*dx*diff);
		if (e < 0 ) { buffer[i].y = buffer[i-1].y; diff = 0; }
		else { buffer[i].y = buffer[i-1].y + sy; diff =  1; }
	}

	if (isRevert)
	{
		//swap x and y axes
		for (int i = 0; i < steps; ++i)
		{
			int temp = buffer[i].x;
			buffer[i].x = buffer[i].y;
			buffer[i].y = temp;
		}
	}

	return steps;
}

//Èý½ÇÐÎÌî³äËã·¨
void scanTriangle_top(FillData* data, float left, float right, float k1, float k2, int ymin, bool pushFirstLine)
{
	if(data->ymin > ymin) data->ymin = ymin;
	for(int y = ymin; ; ++y)
	{
		if(pushFirstLine) {
			data->lines.push_back(Line((int)left, (int)right));
			data->fragmentsCount+=((int)right-(int)left+1);
			pushFirstLine = true;
		}
		left += k1;
		right += k2;
		if (left >= right) break;
	}
}
void scanTriangle_bottom(FillData* data, int x, int y, float k1, float k2, int ymax)
{
	float left, right;
	left = right = (float)x;
	for(int i = y; i <= ymax; ++i)
	{
		data->lines.push_back(Line((int)left,(int)right));
		data->fragmentsCount+=((int)right-(int)left+1);
		left += k1;
		right += k2;
	}
}

void scanTriangle(FillData* data, const Vector& p1, const Vector& p2, const Vector& p3)
{
	int x0 = (int)p1.x; int x1 = (int)p2.x; int x2 = (int)p3.x;
	int y0 = (int)p1.y; int y1 = (int)p2.y; int y2 = (int)p3.y;
	if((x0==x1&&x1==x2)||(y0==y1&&y1==y2))
	{
		return;  
	}  

	if(y0>y1)  
	{
		std::swap(x0,x1);
		std::swap(y0,y1);  
	}  

	if(y0>y2)  
	{  
		std::swap(x0,x2);  
		std::swap(y0,y2);
	}  

	if(y1>y2)  
	{  
		std::swap(y1,y2);
		std::swap(x1,x2);
	}

	if(y0 == y1)
	{
		int left = x0<x1? x0 : x1;
		int right = x0<x1? x1 : x0;
		data->ymin = y0;
		float k1 = (float(y2-y0))/(x2-left);
		float k2 = (float(y2-y0))/(x2-right);
		scanTriangle_top(data, (float)left, (float)right, k1, k2, y0, true);
	}
	else if(y1 == y2)
	{
		data->ymin = y0;
		int left = x1 < x2 ? x1 : x2;
		int right = x1 < x2 ? x2: x1;
		float k1 = (float(y1 - y0))/(left - x0);
		float k2 = (float(y1 - y0))/(right - x0);
		scanTriangle_bottom(data, x0, y0, k1, k2, y1);
	}
	else
	{
		data->ymin = y0;
		if (x1 < x2) 
			scanTriangle_bottom(data, x0, y0, (float(y1-y0))/(x1 - x0), (float(y2-y0))/(x2-x0), y1);
		else 
			scanTriangle_bottom(data, x0, y0,  (float(y2-y0))/(x2-x0), (float(y1-y0))/(x1 - x0), y1);

		float x = x0+ 0.5f + (float)1.0f*(y1-y0)*(x2-x0)/(y2-y0);
		if (x1 < x2)
			scanTriangle_top(data,  (float)x1, x, (float(y2-y1))/(x2 - x1), (float(y2-y0))/(x2-x0), y1, false);
		else 
			scanTriangle_top(data,  x, (float)x1, (float(y2-y0))/(x2 - x0), (float(y2-y1))/(x2-x1), y1, false);
	}
}