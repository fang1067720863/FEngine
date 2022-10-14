

struct VertexPosHWNormalTex
{
	float4	pos			: SV_POSITION;
};

float4 PS(VertexPosHWNormalTex input)
{
    return float4(1.0, 0.0,0.0,0.0);
}