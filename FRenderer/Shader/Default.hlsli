#include"light.hlsli"



cbuffer m_CBFrame : register(b0)
{
	matrix g_View;  // �ý̳�����ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++�����Ԥ�Ƚ��������ת�á�
	float4 g_EyePosW;
}
cbuffer m_CBOnResize : register(b1)
{
	matrix g_Proj; 
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
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION; // �������е�λ��
	float3 NormalW : NORMAL; // �������������еķ���
	float2 Tex : TEXCOORD;
};