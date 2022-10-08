#include"light.hlsli"

cbuffer m_CBFrame : register(b0)
{
	matrix g_View;        // lightView
}
cbuffer m_CBOnResize : register(b1)
{
	matrix g_Proj;       // lightProj
}

cbuffer CBEveryObject : register(b2)
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
    vOut.pos = tmp;
    return vOut;
}


