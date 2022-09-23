
struct VertexIn
{
	float3 PosL    : POSITION;
	float2 TexC    : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float2 TexC    : TEXCOORD;
};

cbuffer CBEveryObject : register(b3)
{
	matrix g_World;
}


VertexOut main(VertexIn input)
{
	VertexOut output;

	output.PosH = mul(float4(input.PosL, 1.0f), g_World);

	// transform to homogeneous clip space.
	output.PosH.x = 2 * output.PosH.x - 1;
	output.PosH.y = 2 * output.PosH.y - 1;

	output.TexC = input.TexC;

	return output;
}

// VertexToPixel main(uint id : SV_VertexID)
// {
// 	// Set up output
// 	VertexToPixel output;

// 	// Calculate the UV (0,0) to (2,2) via the ID
// 	output.uv = float2(
// 		(id << 1) & 2,  // id % 2 * 2
// 		id & 2);

// 	// Adjust the position based on the UV
// 	output.position = float4(output.uv, NEAR_Z_NORM, 1);
// 	output.position.x = output.position.x * 2 - 1;
// 	output.position.y = output.position.y * -2 + 1;

// 	return output;
// }