#include "skybox.hlsli"

TextureCube g_TexCube : register(t0);
SamplerState g_Sam : register(s0);

float4 PS(VertexPosHL pIn) : SV_Target
{
    return g_TexCube.Sample(g_Sam, pIn.PosL);
}
