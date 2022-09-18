#pragma once

#include"Matrix.h"
struct CBChangesEveryFrame
{
	Mat4 view;
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

struct CBDeferred0
{
	Mat4 g_View;
	Mat4 g_ViewInverse;
	Vec4f g_EyePos;
};

struct CBDeferred1
{
	Mat4 g_Proj;
	Mat4 g_ProjInverse;
	float g_Near;
	float g_Far;
	Vec2f padding;
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

struct ShaderInputSlot
{
	struct UniformSlot
	{
		uint32_t slotId;
		bool vsStage{ true };
		bool psStage{ true };
		uint32_t num{ 1 };
		std::string bufferSlot;
	};
	struct TextureSlot
	{
		int32_t slotId{ -1 };
		uint32_t num{ 1 };
		int32_t resourceSlot;
	};
	struct SamplerSlot
	{
		int32_t slotId{ -1 };
		uint32_t num{ 1 };
		int32_t samplerSlot;
	};
	vector<UniformSlot> uniformSlots;
	vector<TextureSlot> textureSlots;
	vector<SamplerSlot> samplerSlots;

	/*int32_t AddShaderReource(const std::string& texture, )
	{
		int32_t resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(texturePath, device);
		mMaterialSlot.textureSlots.push_back(MaterialSlot::TextureSlot{ shaderSlot, 1, resourceSlot });
		return resourceSlot;
	}*/
	

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
