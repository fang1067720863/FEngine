#include"light.hlsli"

//Texture2D g_DiffuseMap : register(t0);
//TextureCube g_TexCube : register(t1);
//SamplerState g_Sam : register(s0);

cbuffer m_CBFrame : register(b0)
{
	matrix g_View;  // �ý̳�����ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++�����Ԥ�Ƚ��������ת�á�
	matrix g_World;
	float4 g_EyePosW;
}
cbuffer m_CBOnResize : register(b1)
{
	matrix g_Proj;  // �ý̳�����ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++�����Ԥ�Ƚ��������ת�á�
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
	float3 PosW : POSITION; // �������е�λ��
	float3 NormalW : NORMAL; // �������������еķ���
	float2 Tex : TEXCOORD;
};