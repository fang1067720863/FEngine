#include"light.hlsli"



cbuffer m_CBFrame : register(b0)
{
	matrix g_View;  
	matrix g_ViewInverse;
	float4 g_EyePos;
}

cbuffer m_CBOnResize : register(b1)
{
    matrix g_Proj; 
    matrix g_ProjInverse;
    float g_Near;
    float g_Far;
}

cbuffer CBChangesRarely : register(b2)
{
	DirectionalLight g_DirLight;
}



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