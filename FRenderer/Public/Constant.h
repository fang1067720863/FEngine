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
