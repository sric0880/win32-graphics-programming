#include "stdafx.h"
#include "Texture2D.h"
#include <cassert>

#include "OutputDebug.h"

extern HINSTANCE hInst;

Texture2D::Texture2D()
	:_hBmp(0),
	_pBits(nullptr),
	bitwise(0)
{
}

Texture2D::~Texture2D()
{
	if (_hBmp != 0) DeleteObject(_hBmp);
	delete(_pBits);
}

Vector Texture2D::sampleTexture2D(float s, float t) const
{
	if (_hBmp == 0) return Vector(0,0,0);
	int pixelY = (1 - t)*(_texture2D.bmHeight - 1);
	int pixelX = s * (_texture2D.bmWidth - 1);
	unsigned char* ptr = (unsigned char*)_pBits + pixelY* _texture2D.bmWidthBytes + pixelX * bitwise;
	float b = 1.0f * ((unsigned int)*ptr++) / 0xff;
	float g = 1.0f * ((unsigned int)*ptr++) / 0xff;
	float r = 1.0f * ((unsigned int)*ptr++) / 0xff;
	return Vector(r, g, b);
}

void Texture2D::loadTexture2D(LPCWSTR imageFileID)
{
	_hBmp = LoadBitmap(hInst, imageFileID);
	GetObject(_hBmp, sizeof(BITMAP), &_texture2D);
	auto size = _texture2D.bmHeight * _texture2D.bmWidthBytes;
	_pBits = new unsigned char[size];
	auto dl = GetBitmapBits(_hBmp, size, _pBits);
	bitwise = _texture2D.bmBitsPixel / 8;
	assert(bitwise >= 3);
}