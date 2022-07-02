#include "Cube.hlsli"

VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.posL, 1.0f);  // mul 才是矩阵乘法, 运算符*要求操作对象为
    vOut.posH = mul(vOut.posH, g_View);               // 行列数相等的两个矩阵，结果为
    vOut.posH = mul(vOut.posH, g_Proj);               // Cij = Aij * Bij
    vOut.Tex = vIn.Tex;                         // 这里alpha通道的值默认为1.0
    return vOut;
}
