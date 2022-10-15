
#include"Pbr.hlsli"
#include"1Default.hlsli"

#define SHADOW 1



Texture2D gBaseColorMap : register(t0);
Texture2D gMetalRoughnessMap : register(t1);
Texture2D gNormalMap : register(t2);
Texture2D gAoMap : register(t3);
Texture2D gEmissiveMap : register(t4);

#if SHADOW
  Texture2D gShadowMap: register(t5);
#endif

SamplerState basicSampler : register(s0);
cbuffer CBChangesEveryObjectDrawing : register(b3)
{
    PbrMaterial g_Material;
}
static const float shadowMapSizeDiv = 1.0/800.0;
static const float shadowDepthBias = 0.005f;
static const int poissonDiskCount = 16;
static const int poissonDiskCount_D2 = 8;

static const float2 poissonDisk[16] = {
    float2( -0.94201624, -0.39906216 ),
    float2( 0.94558609, -0.76890725 ),
    float2( -0.094184101, -0.92938870 ),
    float2( 0.34495938, 0.29387760 ),
    float2( -0.91588581, 0.45771432 ),
    float2( -0.81544232, -0.87912464 ),
    float2( -0.38277543, 0.27676845 ),
    float2( 0.97484398, 0.75648379 ),
    float2( 0.44323325, -0.97511554 ),
    float2( 0.53742981, -0.47373420 ),
    float2( -0.26496911, -0.41893023 ),
    float2( 0.79197514, 0.19090188 ),
    float2( -0.24188840, 0.99706507 ),
    float2( -0.81409955, 0.91437590 ),
    float2( 0.19984126, 0.78641367 ),
    float2( 0.14383161, -0.14100790 )
};

float PCF(float2 uv, float radius,float depth)
{
    float sum = 0.0;
    float shadowFactor =0.0;

    int i;
    // first cheap filtering
    for(i=0;i<poissonDiskCount_D2;i++)
    {
        float2 uvSample = poissonDisk[i]*radius + uv;
        float3 minDepth = gShadowMap.Sample(basicSampler, uvSample).r;
        shadowFactor = depth - minDepth > shadowDepthBias? 1.0f :0.0f;
        sum+=shadowFactor;
    }

    if(sum * (poissonDiskCount_D2 * 1.0 - sum)==0.0)
    {
        return 1 - sum/poissonDiskCount_D2;
    }

    for(i=poissonDiskCount_D2;i<poissonDiskCount;i++)
    {
        float2 uvSample = poissonDisk[i]*radius + uv;
        float3 minDepth = gShadowMap.Sample(basicSampler, uvSample).r;
        shadowFactor = depth - minDepth > shadowDepthBias? 1.0f :0.0f;
        sum+=shadowFactor;
    }
    sum/= poissonDiskCount;
    return 1.0-sum;
}

float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{

    float roughness = 0.0;
    float metallic = 0.0;

    float roughnessFactor = 1.0;
    float metallicFactor = 1.0;
    float3 emissiveFactor = float3(1.0f, 1.0f, 1.0f);

    float3 v = normalize(g_EyePosW.xyz - pIn.PosW);
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
    
    float shadowFactor = 1.0f;
   
    #if SHADOW
        float bias = 0.005f;
        float cell = 5 * shadowMapSizeDiv;
        float4 posLC = mul(float4(pIn.PosW, 1.0f), g_LightTransform);  // light coordinate
        posLC /= posLC.w;
        float2 uv = posLC.xy * float2(0.5f, -0.5f) + float2(0.5f,0.5f);
        shadowFactor = PCF(uv,cell,posLC.z);
    #endif
    
    

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
    color *= shadowFactor;

    // HDR tonemapping
    color = color / (color + float3(1.0f, 1.0f, 1.0f));
    // gamma correct LINEARtoSRGB
    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));
    return float4(color, 1.0f);
}
