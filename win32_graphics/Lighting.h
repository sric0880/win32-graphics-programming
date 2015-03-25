#pragma once
#include "Base.h"

enum LightType{
	Direction,
	Point,
	Spot
};

class Lighting
{
public:
	Lighting();
	~Lighting();

	LightType getType() const { return type; }
	const Vector& getLightDir() const { return lightDir; }
	const Vector& getPositionInCameraSpace() const;
	const Vector& getPosition() const { return position; }
	const Vector& getLightDirInCameraSpace() const;
	float getCutoffAngle() const { return cutoffAngle; }
	const Matrix& getNormalMatrix() const { return normalMatrix; }
	const Vector& getDiffuse() const { return diffuse; }
	const Vector& getAmbient() const { return ambient; }
	const Vector& getSpecular() const { return specular; }
	float getShininess() const { return shininess; }
	bool isPerPixelLighting() const { return usePerPixelLighting; }

	//set direcation in world space
	void setDirection(const Vector& dir);
	void setNormalMatrix(const Matrix& modelviewMatrix);
	void setAmbient(const Vector&);
	void setDiffuse(const Vector&);
	void setSpecular(const Vector&);
	void setShininess(float);
	void setCutoffAngle(float);
	void setType(LightType type);
	//set light position in world space
	void setPosition(const Vector&);
	void setPerPixelLighting(bool);

	void updateViewMatrix(const Matrix* matrix) { viewMatrix = matrix; }
	Vector calculateLighting(const Vector& normal, const Vector& eye) const;

private:
	LightType type;

	Vector lightDir; // direction of light in world space
	Vector lightDirInCameraSpace; // direction of light in camera space
	Vector position; // use for point light or spot light in world space
	Vector positionInCameraSpace; // position of light in camera space

	float cutoffAngle; // use for spot light
	Matrix normalMatrix;  // normal transform to camera space

	Vector diffuse; // diffuse of light
	Vector ambient; // ambient of light
	Vector specular; //specular of light

	const Matrix* viewMatrix;

	float shininess;

	bool usePerPixelLighting;
	bool isLightDirInCSDirty;
};

