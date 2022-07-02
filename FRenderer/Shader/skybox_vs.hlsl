#include "skybox.hlsli"

VertexPosHL VS(VertexPos vIn)
{
    VertexPosHL vOut;

    // ����z = wʹ��z/w = 1(��պб�����Զƽ��)
    float4 posH = mul(float4(vIn.PosL, 1.0f), g_WorldViewProj);
    vOut.PosH = posH.xyww;
    vOut.PosL = vIn.PosL;
    return vOut;
}