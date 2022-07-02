#include "Cube.hlsli"



// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
    return txDiffuse.Sample(samLinear, pIn.Tex);
    //return float4(1.0,1.0,1.0,1.0);
}
