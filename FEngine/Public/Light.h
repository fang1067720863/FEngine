#pragma once

enum LightType
{
	POINT_LIGHT,
	DIRECTION_LIGHT,
	SPOT_LIGHT
};
class Light
{
public:
	LightType mType;
	//Vec4 mAmbient;                           // r, g, b, w
	//Vec4 mDiffuse;                           // r, g, b, w
	//Vec4 mSpecular;                          // r, g, b, w
	//Vec4 mPosition;                          // x, y, z, w
	//Vec4 mDirection;                         // x, y, z
	float _constant_attenuation;             // constant
	float _linear_attenuation;               // linear
	float _quadratic_attenuation;            // quadratic
	float _spot_exponent;                    // exponent
	float _spot_cutoff;                      // spread
};

