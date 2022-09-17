
#include"1_default.hlsli"

struct PixelOutput
{
	float4	albedo						: SV_TARGET0;
	float4	normal						: SV_TARGET1;
	float4	occlusionRoughnessMetallic	: SV_TARGET2;
};
Texture2D gBaseColorMap : register(t0);
Texture2D gMetalRoughnessMap : register(t1);
Texture2D gNormalMap : register(t2);
Texture2D gAoMap : register(t3);
SamplerState basicSampler : register(s0);

PixelOutput PS(VertexPosHWNormalTex pIn) : SV_Target
{
	float3 albedoFromTexture = gBaseColorMap.Sample(basicSampler, pIn.Tex).rgb;
	float3 normalFromTexture = gNormalMap.Sample(basicSampler, pIn.Tex).rgb;
	float3 ormFromTexture = gMetalRoughnessMap.Sample(basicSampler, pIn.Tex).rgb;
	// //if (matData.TextureSrgb[0] == 1)
		//albedoFromTexture = pow(albedoFromTexture, 2.2f);
	//if (matData.TextureSrgb[1] == 1)
		//normalFromTexture = pow(normalFromTexture, 2.2f);
	//if (matData.TextureSrgb[2] == 1)
		//ormFromTexture = pow(ormFromTexture, 2.2f);

	// float4 prevPos = input.prevPos;
	// prevPos = prevPos / prevPos.w;
	// prevPos.xy = prevPos.xy / float2(2.0f, -2.0f) + float2(0.5f, 0.5f);//negate Y because world coord and tex coord have different Y axis.
	// float4 curPos = input.curPos;
	// curPos = curPos / curPos.w;
	// curPos.xy = curPos.xy / float2(2.0f, -2.0f) + float2(0.5f, 0.5f);//negate Y because world coord and tex coord have different Y axis.

	//float3 normal = calculateNormalFromMap(normalFromTexture, normalize(pIn.NormalW), input.tangent);
	
	PixelOutput output;
	output.albedo = float4(albedoFromTexture, 1.0f);;
	output.normal = float4(normalize(pIn.NormalW), 1.0f);

	float roughness = ormFromTexture.g;
	float metal = ormFromTexture.b;
	output.occlusionRoughnessMetallic = float4(0, roughness, metal, 0);
	output.albedo.a = 1.0f;
	return output;
}