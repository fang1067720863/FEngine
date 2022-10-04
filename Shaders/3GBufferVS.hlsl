

#include"3GBuffer.hlsli"


VertexPosHWNormalTex VS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);
    vOut.PosW = posW.xyz;
    float4 tmp = mul(posW, viewProj);
    vOut.PosH = tmp;
    //vOut.NormalW = mul(vIn.NormalL, (float3x3)g_World);
    vOut.NormalW = normalize(vIn.NormalL);
    vOut.Tex = vIn.Tex;
    return vOut;
}
