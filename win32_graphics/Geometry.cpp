#include "stdafx.h"
#include "Geometry.h"
#include <cmath>
#include <bitset>
#include <list>
//#include <forward_list>

//bool crossPlane(float destx, float f1, float f2, float& u, float f3, float f4, float f5, float f6)
//{
//	u = (destx - f1) / (f2 - f1);
//	if (u > 0 && u < 1)
//	{
//		float r = f3 + (f4 - f3)*u;
//		if (r > -1 && r < 1)
//		{
//			r = f5 + (f6 - f5)*u;
//			if (r > -1 && r < 1) return true;
//		}
//	}
//	return false;
//}
//int clippingLine(const Vertex* v1, const Vertex* v2, float* linearValues)
//{
//	int c = 0;
//	float u = 0;
//	if (crossPlane(-1, v1->position.x, v2->position.x, u, v1->position.y, v2->position.y, v1->position.z, v2->position.z))
//	{
//		linearValues[c++] = u;
//	}
//	if (crossPlane(1, v1->position.x, v2->position.x, u, v1->position.y, v2->position.y, v1->position.z, v2->position.z))
//	{
//		linearValues[c++] = u;
//		if (c == 2) return c;
//	}
//	if (crossPlane(-1, v1->position.y, v2->position.y, u, v1->position.x, v2->position.x, v1->position.z, v2->position.z))
//	{
//		linearValues[c++] = u;
//		if (c == 2) return c;
//	}
//	if (crossPlane(1, v1->position.y, v2->position.y, u, v1->position.x, v2->position.x, v1->position.z, v2->position.z))
//	{
//		linearValues[c++] = u;
//		if (c == 2) return c;
//	}
//	if (crossPlane(-1, v1->position.z, v2->position.z, u, v1->position.y, v2->position.y, v1->position.x, v2->position.x))
//	{
//		linearValues[c++] = u;
//		if (c == 2) return c;
//	}
//	if (crossPlane(1, v1->position.z, v2->position.z, u, v1->position.y, v2->position.y, v1->position.x, v2->position.x))
//	{
//		linearValues[c++] = u;
//		if (c == 2) return c;
//	}
//	return c;
//}
inline void clippingTriangle_constructBitSet(const Vector& position, std::bitset<6> & code)
{
	if (position.x < -1) code[0] = 1;	//left
	if (position.x > 1) code[1] = 1;		//right
	if (position.y > 1) code[2] = 1;		//up
	if (position.y < -1) code[3] = 1;	//down
	if (position.z < -1) code[4] = 1;	//near
	if (position.z > 1) code[5] = 1;		//far
}
//void clippingTriangle_(std::bitset<6>& code1, std::bitset<6>& code2, const Vertex* v1, const Vertex* v2, Vertex* out, int& c)
//{
//	if ( code1.none() ) { //inside box
//		out[c++] = *v1;
//	}
//	if ((code1 & code2) == 0) { // maybe cross box
//		float linearValues[2] = {0};
//		int dotCount = clippingLine(v1, v2, linearValues);
//		if (dotCount == 2 && linearValues[1] < linearValues[0]) std::swap(linearValues[0], linearValues[1]);
//		for (int i = 0; i < dotCount; ++i)
//		{
//			out[c].position = v1->position + (v2->position - v1->position) * linearValues[i];
//			out[c].color = v1->color + (v2->color - v1->color) * linearValues[i];
//			out[c].normal = v1->normal + (v2->normal - v1->normal) * linearValues[i];
//			out[c].texCoord = v1->texCoord + (v2->texCoord - v1->texCoord) * linearValues[i];
//			out[c].eye = v1->eye + (v2->eye - v1->eye) * linearValues[i];
//			++c;
//		}
//	}
//}

struct Node{
	std::bitset<6> code;
	Vector weight;
	Vector pos;
};

//Sutherland Hodgman Algorithm
int clippingTriangle(const Vertex* v1, const Vertex* v2, const Vertex* v3, Vertex* out)
{
	Node n1,n2,n3;
	clippingTriangle_constructBitSet(v1->position, n1.code);
	clippingTriangle_constructBitSet(v2->position, n2.code);
	clippingTriangle_constructBitSet(v3->position, n3.code);
	//test
	auto a1 = n1.code | n2.code | n3.code;
	if (a1.none()) //all in box
	{
		out[0] = *v1;
		out[1] = *v2;
		out[2] = *v3;
		return 3;
	}
	auto a2 = n1.code & n2.code & n3.code;
	if (a2 != 0) return 0; //all out of box

	n1.weight = { 1, 0, 0 };
	n2.weight = { 0, 1, 0 };
	n3.weight = { 0, 0, 1 };
	n1.pos = v1->position;
	n2.pos = v2->position;
	n3.pos = v3->position;

	std::list<Node> l;
	l.push_front(n3);
	l.push_front(n2);
	l.push_front(n1);
	if (a1.at(2)) // if up
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(2))
			{
				float w = (1 - cur->pos.y) / (iter->pos.y - cur->pos.y);
				Node n;
				n.pos.x = (1 - w)*cur->pos.x + w * iter->pos.x;
				n.pos.z = (1 - w)*cur->pos.z + w * iter->pos.z;
				n.pos.y = 1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}

		//delete node where is up of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(2)) iter = l.erase(iter);
			else ++iter;
		}
	}

	if (a1.at(3)) // if down
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(3))
			{
				float w = (-1 - cur->pos.y) / (iter->pos.y - cur->pos.y);
				Node n;
				n.pos.x = (1 - w)*cur->pos.x + w * iter->pos.x;
				n.pos.z = (1 - w)*cur->pos.z + w * iter->pos.z;
				n.pos.y = -1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}

		//delete node where is down of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(3)) iter = l.erase(iter);
			else ++iter;
		}

	}

	if (a1.at(4)) // if front
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(4))
			{
				float w = (-1 - cur->pos.z) / (iter->pos.z - cur->pos.z);
				Node n;
				n.pos.x = (1 - w)*cur->pos.x + w * iter->pos.x;
				n.pos.y = (1 - w)*cur->pos.y + w * iter->pos.y;
				n.pos.z = -1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}

		//delete node where is front of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(4)) iter = l.erase(iter);
			else ++iter;
		}

	}

	if (a1.at(5)) // if back
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(5))
			{
				float w = (1 - cur->pos.z) / (iter->pos.z - cur->pos.z);
				Node n;
				n.pos.x = (1 - w)*cur->pos.x + w * iter->pos.x;
				n.pos.y = (1 - w)*cur->pos.y + w * iter->pos.y;
				n.pos.z = 1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}

		//delete node where is back of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(5)) iter = l.erase(iter);
			else ++iter;
		}

	}

	if (a1.at(0)) // if left
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(0))
			{
				float w = (-1 - cur->pos.x) / (iter->pos.x - cur->pos.x);
				Node n;
				n.pos.y = (1 - w)*cur->pos.y + w * iter->pos.y;
				n.pos.z = (1 - w)*cur->pos.z + w * iter->pos.z;
				n.pos.x = -1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}

		//delete node where is left of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(0)) iter = l.erase(iter);
			else ++iter;
		}

	}

	if (a1.at(1)) // if right
	{
		auto iter = l.begin();
		while (1)
		{
			auto cur = iter++;
			if (iter == l.end()) iter = l.begin(); //cycle

			if ((cur->code ^ iter->code).at(1))
			{
				float w = (1 - cur->pos.x) / (iter->pos.x - cur->pos.x);
				Node n;
				n.pos.y = (1 - w)*cur->pos.y + w * iter->pos.y;
				n.pos.z = (1 - w)*cur->pos.z + w * iter->pos.z;
				n.pos.x = 1;
				clippingTriangle_constructBitSet(n.pos, n.code);
				n.weight = (1 - w) * cur->weight + w * iter->weight;
				if (iter != l.begin())
					l.insert(iter, n);
				else
					l.insert(l.end(), n);
			}

			if (iter == l.begin()) break;
		}
		//delete node where is right of box
		for (iter = l.begin(); iter != l.end();)
		{
			if (iter->code.at(1)) iter = l.erase(iter);
			else ++iter;
		}
	}

	int c = 0;
	for (auto iter = l.begin(); iter != l.end(); ++iter)
	{
		out[c].position = iter->pos;
		out[c].color = v1->color * iter->weight.x + v2->color * iter->weight.y + v3->color * iter->weight.z;
		out[c].normal = v1->normal * iter->weight.x + v2->normal * iter->weight.y + v3->normal * iter->weight.z;
		out[c].texCoord = v1->texCoord * iter->weight.x + v2->texCoord * iter->weight.y + v3->texCoord * iter->weight.z;
		out[c].eye = v1->eye * iter->weight.x + v2->eye * iter->weight.y + v3->eye * iter->weight.z;
		++c;
	}
	return c;

	/*int c = 0;
	clippingTriangle_(code[0], code[1], v1, v2, out, c);
	clippingTriangle_(code[1], code[2], v2, v3, out, c);
	clippingTriangle_(code[2], code[0], v3, v1, out, c);
	return c;*/
}

//see http://en.wikipedia.org/wiki/Back-face_culling
bool isBackface(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	Vector n = (v2.position - v1.position).crossProduct(v3.position - v1.position);
	// v1 already in camera space
	return v1.eye.dotProduct(n) > 0;
}
///////
//Draw line agorithms (not contain start and end)
int drawLineBresenham(Fragment* buffer, const Fragment* start, const Fragment* end)
{
	int start_x = start->x;
	int start_y = start->y;
	int end_x = end->x;
	int end_y = end->y;
	int dx = end_x - start_x;
	int dy = end_y - start_y;
	if(dx == 0 && dy ==0) return 0;
	bool isRevert = false;
	if (dx == 0 || fabs( ((float)dy)/dx ) > 1 )
	{
		//swap x and y axes
		std::swap(start_x, start_y);
		std::swap(end_x, end_y);
		isRevert = true;
	}
	if (start_x > end_x){
		std::swap(start_x, end_x);
		std::swap(start_y, end_y);
	}
	dx = end_x - start_x;
	dy = end_y - start_y;

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

// scan-line algorithm
void scanTriangle_top(FillData* data, float left, float right, float k1, float k2, int ymin, bool pushFirstLine)
{
	if(data->ymin > ymin) data->ymin = ymin;
	for(int y = ymin; ; ++y)
	{
		if(pushFirstLine) {
			int l = std::ceilf(left);
			int r = std::floorf(right);
			data->lines.push_back(Line(l, r));
			data->fragmentsCount+=(r-l+1);
		}
		pushFirstLine = true;
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
		int l = std::ceilf(left);
		int r = std::floorf(right);
		data->lines.push_back(Line(l,r));
		data->fragmentsCount+=(r-l+1);
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
		float k1 = (float(x2-left)/(y2-y0));
		float k2 = (float(x2-right))/(y2-y0);
		scanTriangle_top(data, (float)left, (float)right, k1, k2, y0, true);
	}
	else if(y1 == y2)
	{
		data->ymin = y0;
		int left = x1 < x2 ? x1 : x2;
		int right = x1 < x2 ? x2: x1;
		float k1 = (float(left - x0))/(y1 - y0);
		float k2 = (float(right - x0))/(y1 - y0);
		scanTriangle_bottom(data, x0, y0, k1, k2, y1);
	}
	else
	{
		data->ymin = y0;
		float k1 = (float(x1 - x0)) / (y1 - y0);
		float k2 = (float(x2 - x0)) / (y2 - y0);
		if (k1 < k2) 
			scanTriangle_bottom(data, x0, y0, k1, k2, y1);
		else
			scanTriangle_bottom(data, x0, y0, k2, k1, y1);

		float x = x0 + (float)1.0f*(y1-y0)*(x2-x0)/(y2-y0);
		if (x1 < x)
			scanTriangle_top(data,  (float)x1, x, (float(x2 - x1))/(y2-y1), (float(x2-x0))/(y2-y0), y1, false);
		else 
			scanTriangle_top(data,  x, (float)x1, (float(x2 - x0))/(y2-y0), (float(x2-x1))/(y2-y1), y1, false);
	}
}

