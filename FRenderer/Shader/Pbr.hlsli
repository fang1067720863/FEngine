
#include"Default.hlsli"

#define PI 3.14159265359f


float3 fresnel_GGX(float VdotH, float3 f0)
{
    return f0 + (1 - f0) * pow(1 - VdotH, 5);
}

float3 fresnel_Epic(float VdotH, float3 f0)
{
    return f0 + (1 - f0) * exp2((-5.55473 * VdotH - 6.98316) * VdotH);
}

float microfacetDistribution(float NdotH, float roughness)
{
    float roughness2 = roughness * roughness;
    float f = NdotH * NdotH * (roughness2 - 1) + 1.0;
    return roughness2 / (PI * f * f);
}

float geometricOcclusion(float NdotL, float NdotV, float roughness)
{
    // End result of remapping:
    float k = pow(roughness + 1, 2) / 8.0f;
    float attenuationV = NdotV / (NdotV * (1 - k) + k);
    float attenuationL = NdotL / (NdotL * (1 - k) + k);
    return attenuationL * attenuationV;
}

float3 BRDF_Diffuse_Lambert(float3 albedo)
{
    return albedo / PI;
}

float3 BRDF_Diffuse_Disney(float VdotH, float NdotL, float NdotV,
    float roughness, float3 diffuseColor)
{
    float Fd90 = 0.5 + 2.0 * roughness * VdotH * VdotH;
    float3 f0 = float3(0.1f, 0.1f, 0.1f);
    float3 invF0 = float3(1.0, 1.0, 1.0) - f0;
    float dim = min(invF0.r, min(invF0.g, invF0.b));
    float result = ((1.0 + (Fd90 - 1.0) * pow(1.0 - NdotL, 5.0)) * (1.0 + (Fd90 - 1.0) * pow(1.0 - NdotV, 5.0))) * dim;
    return diffuseColor * result;
}

float3 BRDF_Diffuse_Burley(float VdotH, float NdotL, float NdotV,
    float roughness, float3 diffuseColor)
{
    float energyBias = lerp(roughness, 0.0, 0.5);            // 0.5
    float energyFactor = lerp(roughness, 1.0, 1.0 / 1.51);   // 1/3.1415
    float fd90 = energyBias + 2.0 * VdotH * VdotH * roughness;
    float f0 = 1.0;
    float lightScatter = f0 + (fd90 - f0) * pow(1.0 - NdotL, 5.0);
    float viewScatter = f0 + (fd90 - f0) * pow(1.0 - NdotV, 5.0);
    float result = lightScatter * viewScatter * energyFactor;
    return diffuseColor * result;
}

float3 CookTorranceSpecular(float VdotH, float NdotH, float NdotL, float NdotV,
    float roughness, float metalness, float3 f0, out float3 kS)
{

    float D = microfacetDistribution(NdotH, roughness);
    float3 F = fresnel_Epic(VdotH, f0);
    float G = geometricOcclusion(NdotL, NdotV, roughness);
    return (D * F * G) / (4 * max(NdotV * NdotL, 0.01f));
}
float4 SRGBtoLINEAR(float4 srgbIn)
{
    float3 linOut = pow(srgbIn.xyz, float3(2.2f, 2.2f, 2.2f));
    return float4(linOut, srgbIn.w);
}

float4 LINEARtoSRGB(float4 srgbIn)
{
    float3 linOut = pow(srgbIn.xyz, float3(1.0 / 2.2f, 1.0 / 2.2f, 1.0 / 2.2f ));
    return float4(linOut, srgbIn.w);
}

float3 BRDF(float VdotH,
    float NdotH,
    float NdotL,
    float NdotV,
    float roughness,
    float metalness,
    float3 f0,
    float3 diffuseColor,
    float3 lightColor,
    float ao,
    float3 emissive)
{


    float D = microfacetDistribution(NdotH, roughness);
    float3 F = fresnel_Epic(VdotH, f0);
    float G = geometricOcclusion(NdotL, NdotV, roughness);

    float3 specContrib = (D * F * G) / (4 * max(NdotV * NdotL, 0.01f));   //CookTorranceSpecular

    float3 diffuse = BRDF_Diffuse_Disney(VdotH, NdotL, NdotV, roughness, diffuseColor);
    float3 diffuseContrib = (float3(1.0f, 1.0f, 1.0f) - F)  * diffuse;
    float3 color = NdotL * lightColor * (diffuseContrib + specContrib);

    color *= ao;
    color += emissive;

    //#ifdef EMISSIVE_MAP
    //    vec3 emissive = SRGBtoLINEAR(texture(emissiveMap, texCoord0)).rgb * pbr.emissiveFactor.rgb;
    //#else
    //    vec3 emissive = pbr.emissiveFactor.rgb;
    //#endif
    //    color += emissive;

    return color;
}
