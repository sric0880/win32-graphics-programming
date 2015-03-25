#include "stdafx.h"
#include "Lighting.h"
#include <cmath>
#include "Geometry.h"

Lighting::Lighting()
	:type(Direction),
	cutoffAngle(30),
	lightDir(0, -1, 0),
	position(0, 5, 0),
	shininess(5.0f),
	usePerPixelLighting(true),
	viewMatrix(nullptr),
	isLightDirInCSDirty(true)
{
}

Lighting::~Lighting()
{
}

void Lighting::setDirection(const Vector& dir)
{
	lightDir = dir;
	isLightDirInCSDirty = true;
	lightDir.normalize();
}

void Lighting::setNormalMatrix(const Matrix& modelviewMatrix)
{
	normalMatrix = ~modelviewMatrix;
	Matrix::Tranpose(normalMatrix);
}

void Lighting::setAmbient(const Vector& a)
{
	ambient = a;
}
void Lighting::setDiffuse(const Vector& d)
{
	diffuse = d;
}
void Lighting::setSpecular(const Vector& s)
{
	specular = s;
}
void Lighting::setShininess(float sh)
{
	shininess = sh;
}
void Lighting::setCutoffAngle(float ca)
{
	cutoffAngle = ca;
}
void Lighting::setType(LightType t)
{
	type = t;
}
void Lighting::setPosition(const Vector& p)
{
	
	position = p;
}
void Lighting::setPerPixelLighting(bool use)
{
	usePerPixelLighting = use;
}

const Vector& Lighting::getPositionInCameraSpace() const 
{
	if (viewMatrix)
	{
		const_cast<Lighting*>(this)->positionInCameraSpace = *viewMatrix * position;
	}
	return positionInCameraSpace; 
}
const Vector& Lighting::getLightDirInCameraSpace() const 
{ 
	if (viewMatrix)
	{
		if (isLightDirInCSDirty)
		{
			auto ts = const_cast<Lighting*>(this);
			ts->lightDirInCameraSpace = *viewMatrix * lightDir - *viewMatrix * Vector::zero;
			ts->lightDirInCameraSpace.normalize();
			ts->isLightDirInCSDirty = false;
		}
	}
	return lightDirInCameraSpace;
}

Vector Lighting::calculateLighting(const Vector& normal, const Vector& eye) const
{
	auto& lightDirInCameraSpace = getLightDirInCameraSpace();
	auto L = -1 * lightDirInCameraSpace;
	//the max prevents the negative values
	float intensity = max(normal.dotProduct(L), 0.0f);
	if (intensity > 0.0f)
	{
		Vector h = L + eye;
		h.normalize();
		float intSpec = max(h.dotProduct(normal), 0.0f);
		Vector spec = specular * pow(intSpec, shininess);
		Vector color = intensity * diffuse + spec;
		if (color.x > ambient.x || color.y > ambient.y || color.z > ambient.z)
		{
			clerp(0, 1, color);
			return color;
		}
		else return ambient;
		
	}
	else
		return ambient;
}