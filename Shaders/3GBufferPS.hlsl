
#include"3GBuffer.hlsli"

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

float3 CalculateNormalInWorld(float3 normalFromTexture, float3 normal, float3 tangent)
{
   	float3 unpackedNormal = normalFromTexture * 2.0f - 1.0f;
	float3 N = normal;
	float3 T = normalize(tangent - N * dot(tangent, N));
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	return normalize(mul(unpackedNormal, TBN));
}

float3 NormalInWorldWithoutTangent(float3 normalFromTexture, float3 normal, float3 eyePos,float2 uv)
{
    // Perturb normal, see http://www.thetenthplanet.de/archives/1180
    float3 unpackedNormal = normalFromTexture * 2.0f - 1.0f;

    float3 q1 = ddx(eyePos);
    float3 q2 = ddy(eyePos);
    float2 st1 = ddx(uv);
    float2 st2 = ddy(uv);

    float3 N = normalize(normal);
    float3 T = float3(0.0,0.0,1.0);
    //normalize(q1 * st2.y - q2 * st1.y);
    float3 B = normalize(cross(N, T));

	float3x3 TBN = float3x3(T, B, N);
	return normalize(mul(unpackedNormal, TBN));

}
PixelOutput PS(VertexPosHWNormalTex pIn) : SV_Target
{
	float3 albedoFromTexture = gBaseColorMap.Sample(basicSampler, pIn.Tex).rgb;
	float3 normalFromTexture = gNormalMap.Sample(basicSampler, pIn.Tex).rgb;
	float3 ormFromTexture = gMetalRoughnessMap.Sample(basicSampler, pIn.Tex).rgb;
	float ao = gAoMap.Sample(basicSampler, pIn.Tex).r;

	float3 normalWC = NormalInWorldWithoutTangent(normalFromTexture, pIn.NormalW, g_EyePosW.xyz, pIn.Tex);

	PixelOutput output;
	output.albedo = float4(albedoFromTexture, 1.0f);;
	output.normal = float4(normalWC, 1.0f);

	float roughness = ormFromTexture.g;
	float metal = ormFromTexture.b;
	output.occlusionRoughnessMetallic = float4(ao, roughness, metal, 0);
	output.albedo.a = 1.0f;
	return output;
}