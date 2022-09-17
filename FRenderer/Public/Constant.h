#pragma once

#include"Matrix.h"
struct CBChangesEveryFrame
{
	Mat4 view;
	//Mat4 world;
	Vec4f eyePos;
};

struct CBChangesOnResize
{
	Mat4 proj;
};
struct CBEveryObject
{
	Mat4 world;
};


struct Light
{
	Vec4f ambient;
	Vec4f diffuse;
	Vec4f specular;
	Vec4f direction;
};

struct SpotLight
{
	Vec4f Color;
	Vec4f Direction;
	Vec3f Position;
	float Range;
	float SpotlightAngle;
};

struct DirectionalLight
{
	Vec4f AmbientColor;
	Vec4f DiffuseColor;
	Vec3f Direction;
	float Intensity;
};

class BumLight
{
public:
	DirectionalLight GetInternalData() {
		return data._light;
	}

	union LightData
	{
		DirectionalLight _light;
	}data;
	

};
