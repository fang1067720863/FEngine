#include"light.hlsli"

//Texture2D g_DiffuseMap : register(t0);
//TextureCube g_TexCube : register(t1);
//SamplerState g_Sam : register(s0);

cbuffer m_CBFrame : register(b0)
{
	matrix g_View;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
	matrix g_World;
	float4 g_EyePosW;
}
cbuffer m_CBOnResize : register(b1)
{
	matrix g_Proj;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
}

//cbuffer CBChangesEveryObjectDrawing : register(b2)
//{
//	Material g_Material;
//}
//cbuffer CBChangesRarely : register(b3)
//{
//	DirectionalLight g_DirLight;
//}


struct VertexPosNormalTex
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION; // 在世界中的位置
	float3 NormalW : NORMAL; // 法向量在世界中的方向
	float2 Tex : TEXCOORD;
};