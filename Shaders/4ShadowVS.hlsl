#include"light.hlsli"

cbuffer m_CBFrame : register(b0)
{
	matrix g_View;        // lightView
	float4 g_EyePosW;     // lightPos
}
cbuffer m_CBOnResize : register(b1)
{
	matrix g_Proj;       // lightProj
}
cbuffer CBChangesRarely : register(b2)
{
	DirectionalLight g_DirLight;
}
cbuffer CBEveryObject : register(b3)
{
	matrix g_World;
}




struct VertexPosNormalTex
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
	float4	pos			: SV_POSITION;
};

VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    float4 tmp = mul(posW, viewProj);
    return vOut;
}


