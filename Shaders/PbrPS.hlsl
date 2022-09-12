
#include"Pbr.hlsli"




Texture2D gBaseColorMap : register(t0);
Texture2D gMetalRoughnessMap : register(t1);
Texture2D gNormalMap : register(t2);
Texture2D gAoMap : register(t3);
Texture2D gEmissiveMap : register(t4);
SamplerState basicSampler : register(s0);
cbuffer CBChangesEveryObjectDrawing : register(b3)
{
    PbrMaterial g_Material;
}

float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{

    float roughness = 0.0;
    float metallic = 0.0;

    float roughnessFactor = 1.0;
    float metallicFactor = 1.0;
    float3 emissiveFactor = float3(1.0f, 1.0f, 1.0f);

        /* float4 baseColorFactor = g_Material
     float3 emissiveFactor;

     float metallicFactor;
     float roughnessFactor;
     float alphaMask;
     float alphaMaskCutoff;
     float aoStrength;*/


    float3 v = normalize(g_EyePosW - pIn.PosW);
    float3 n = normalize(pIn.NormalW);

    float4 baseColor = gBaseColorMap.Sample(basicSampler, pIn.Tex);
    float4 mrSample = gMetalRoughnessMap.Sample(basicSampler, pIn.Tex);
    float ao = gAoMap.Sample(basicSampler, pIn.Tex).r;
    float3 emissive = gEmissiveMap.Sample(basicSampler, pIn.Tex).rgb;
    
    roughness = mrSample.g * roughnessFactor;
    metallic = mrSample.b * metallicFactor;
    emissive *= emissiveFactor;


    float3 f0 = float3(0.04, 0.04, 0.04);
    float3 diffuseColor = baseColor.rgb * (float3(1.0f, 1.0f, 1.0f) - f0);
    diffuseColor *= 1.0 - metallic;
  
    
    float3 Lo = float3(0.0,0.0,0.0);

    float3 lightColor = g_DirLight.Diffuse.rgb;
    float3 l = g_DirLight.Direction.xyz;
    float3 h = normalize(l + v);

    float VdotH = max(dot(h, v), 0.0f);
    float NdotH = max(dot(n, h), 0.0f);
    float NdotL = max(dot(n, l), 0.0f);
    float NdotV = max(dot(n, v), 0.0f);

    Lo += BRDF(VdotH, NdotH, NdotL, NdotV, roughness, metallic, f0, diffuseColor, lightColor, ao, emissive);

   /* for (int i = 0; i < lightNum; ++i)
    {

        float4 lightColor = lightData.values[index++];
        float3 direction = -lightData.values[index++].xyz;

        float3 l = direction;
        float3 h = normalize(l + v);

        float VdotH = max(dot(h, v), 0.0f);
        float NdotH = max(dot(n, h), 0.0f);
        float NdotL = max(dot(n, l), 0.0f);
        float NdotV = max(dot(n, v), 0.0f);
            
        Lo += BRDF(
                    VdotH,
                    NdotH,
                    NdotL,
                    NdotV
                    roughness,
                    metalness,
                    f0,
                    albedo,
                    lightColor,
                    ao
                   )
    }*/

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //float3 ambient = float3(0.03, 0.03, 0.03) * albedo * ao;

    float3 color = Lo.rgb;

    // HDR tonemapping
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    // gamma correct LINEARtoSRGB
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    return float4(color, 1.0f);
}
