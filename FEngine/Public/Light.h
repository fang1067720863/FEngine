#pragma once

#include"Reference.h"
#include"Vec.h"
#include"Node.h"
enum LightType
{
	POINT_LIGHT,
	DIRECTION_LIGHT,
	SPOT_LIGHT
};
struct SpotLightData
{
	Vec4f ambient;                           // r, g, b, w
	Vec4f diffuse;                           // r, g, b, w
	Vec4f specular;                          // r, g, b, w

	Vec4f direction;
	Vec3f position;
	float innerAngle;

	float outerAngle;
	float _constant_attenuation;             // constant
	float _linear_attenuation;               // linear
	float _quadratic_attenuation;            // quadratic
};

struct DirLightData
{
	Vec4f ambient;                           // r, g, b, w
	Vec4f diffuse;                           // r, g, b, w
	Vec4f specular;                          // r, g, b, w

	Vec4f direction;

	float _constant_attenuation;             // constant
	float _linear_attenuation;               // linear
	float _quadratic_attenuation;            // quadratic
	float pad;
};

struct PointLightData
{
	Vec4f ambient;                           // r, g, b, w
	Vec4f diffuse;                           // r, g, b, w
	Vec4f specular;                          // r, g, b, w


	Vec3f position;
	float innerAngle;

	float outerAngle;
	float _constant_attenuation;             // constant
	float _linear_attenuation;               // linear
	float _quadratic_attenuation;            // quadratic
};


class BasicLight: public FReference
{
public:

	Vec4f ambient;                           // r, g, b, w
	Vec4f diffuse;                           // r, g, b, w
	Vec4f specular;                          // r, g, b, w

	float _constant_attenuation;             // constant
	float _linear_attenuation;               // linear
	float _quadratic_attenuation;            // quadratic
	

	virtual void* GetData() { return nullptr; }
};

class DirectLight : public BasicLight
{
public:
	Vec4f direction = Vec4f(0.0, 0.0, -1.0, 0.0);

};



class SpotLight : public BasicLight
{

	Vec3f position = Vec3f(0.0f, 0.0f, 0.0f);
	Vec4f direction = Vec4f(0.0, 0.0, -1.0, 0.0);
	double innerAngle = RadiansToDegrees(30.0);
	double outerAngle = RadiansToDegrees(45.0);

	virtual void* GetData() {
	 
		//return  &SpotLightData{}
	}
};
class PointLight : public BasicLight
{
	Vec4f position = Vec4f(0.0f, 5.0f, 0.0f, 0.0f);
};
class LightManager
{
public:

	static LightManager& Instance()
	{
		static LightManager instance;
		return instance; 
	}
	Ptr<DirectLight> GetDirectLight0() { return _directLights[0]; }
	

	Ptr<DirectLight> CreateDirectLight() 
	{ 
		Ptr<DirectLight> light = new DirectLight();
		_directLights.push_back(light); 
		return light;
	}
private:
	LightManager()
	{

	}

	std::vector<Ptr<DirectLight>> _directLights;
	std::vector<Ptr<PointLight>> _pointLights;
	std::vector<Ptr<SpotLight>> _spotLights;
};

