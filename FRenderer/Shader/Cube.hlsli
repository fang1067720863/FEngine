
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer m_CBFrame : register(b0)
{
	matrix g_View;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
    float4 eyePos;  // 可以在前面添加row_major表示行主矩阵
 
}
cbuffer m_CBOnResize : register(b1)
{
 
    matrix g_Proj;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
}

cbuffer ConstantBuffer : register(b2)
{
	matrix g_World1; // matrix可以用float4x4替代。不加row_major的情况下，矩阵默认为列主矩阵，
	matrix g_View1;  // 可以在前面添加row_major表示行主矩阵
	matrix g_Proj1;  // 该教程往后将使用默认的列主矩阵，但需要在C++代码端预先将矩阵进行转置。
}




struct VertexIn
{
	float3 posL : POSITION;
	float2 Tex : TEXCOORD0;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 Tex : TEXCOORD0;
};
