#pragma once
#include "Base.h"

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	Vector sampleTexture2D(float s, float t) const;
	void loadTexture2D(LPCWSTR imageFile);
	bool isEmpty() const { return _hBmp == 0; }

private:
	HBITMAP _hBmp;
	BITMAP _texture2D;
	unsigned char* _pBits;
	int bitwise;
};

