
#include"Pbr.hlsli"
#include"3Deferred.hlsli"
// draw triangle



Texture2D gBaseColorMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gOrmMap : register(t2);   // occlusionRoughnessMetallic


Texture2D gDepthBuffer				: register(t3);
SamplerState basicSampler : register(s0);

// cbuffer CBEveryObject : register(b3)
// {
// 	matrix g_World;
//     matrix g_ViewInverse;
//     matrix g_ProjInverse;
//     float g_Near;
//     float g_Far;
//     float g_EyePos;
// }


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

struct PixelOutput
{
	float4	color				: SV_TARGET0;
	float4	ambientSpecular		: SV_TARGET1;
};

float LinearDepth(float depth)
{
    return (depth * g_Near) / (g_Far - depth * (g_Far - g_Near));
}

float3 ProjToWorld(float2 uv, float depthProj)
{
    float ndcX = uv.x * 2 -1;
    float ndcY = 1- uv.y * 2; 
    float4 posView = mul(float4(ndcX,ndcY,depthProj,1.0), g_ProjInverse);
    posView = posView /posView.w;
    return mul(posView, g_ViewInverse).xyz;
}
float4 PS(VertexToPixel pIn)
{
    float depthProj = gDepthBuffer.Sample(basicSampler, pIn.uv);
    float depthView = LinearDepth(depthProj);
    float depth = (depthView - g_Near)/(g_Far - g_Near);  //[0,1]
    if (depth<=0.0)
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    
    float4 posWorld =  ProjToWorld(pIn.uv, depthProj);

    float3 n = gNormalMap.Sample(basicSampler, pIn.uv).xyz;
    float4 baseColor = gBaseColorMap.Sample(basicSampler, pIn.uv);
    float4 orm = gOrmMap.Sample(basicSampler, pIn.uv);
    float occlusion = orm.x;
    float metallic = orm.z;
    float roughness = orm.y;

    float3 v = g_EyePos - posWorld;
    float3 l = - g_DirLight.Direction.xyz;
    float3 h = normalize(l + v);

    float3 lightColor = g_DirLight.Diffuse.rgb;
    float3 f0 = float3(0.04, 0.04, 0.04);
    float3 emissive = float3(1.0, 1.0, 1.0);

     float3 diffuseColor = baseColor.rgb * (float3(1.0f, 1.0f, 1.0f) - f0);
    diffuseColor *= 1.0 - metallic;

    float VdotH = max(dot(h, v), 0.0f);
    float NdotH = max(dot(n, h), 0.0f);
    float NdotL = max(dot(n, l), 0.0f);
    float NdotV = max(dot(n, v), 0.0f);

    float3 Lo = float3(0.0,0.0,0.0);
    Lo += BRDF(VdotH, NdotH, NdotL, NdotV, roughness, metallic, f0, diffuseColor, lightColor, occlusion, emissive);

     float3 color = Lo.rgb;
    // HDR tonemapping
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    // gamma correct LINEARtoSRGB
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    return float4(color, 1.0f);
    
    
    return float4(1.0,1.0,1.0,1.0);
}