#include "Triangle.hlsli"

// 顶点着色器
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.pos, 1.0f);
    return vOut;
}
