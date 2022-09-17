#include"1Default.hlsli"


VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    //float4 posW = float4(vIn.PosL, 1.0f);
    vOut.PosW = posW.xyz;
    float4 tmp = mul(posW, viewProj);
    vOut.PosH = tmp;
    vOut.NormalW = mul(vIn.NormalL, (float3x3)g_World);
    vOut.NormalW = normalize(vOut.NormalW);
    vOut.Tex = vIn.Tex;
    return vOut;
}


