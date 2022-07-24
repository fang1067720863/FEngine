#include"Default.hlsli"





// ������ɫ��(3D)
float4 PS(VertexPosHWNormalTex pIn) : SV_Target
{
    // ����ʹ��������ʹ��Ĭ�ϰ�ɫ
    float4 texColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //texColor = g_DiffuseMap.Sample(g_Sam, pIn.Tex);
   
    // ��׼��������
    float3 normal= normalize(pIn.NormalW);

    // �������ָ���۾����������Լ��������۾��ľ���
    float3 toEyeW = normalize(g_EyePosW - pIn.PosW);
    float distToEye = distance(g_EyePosW, pIn.PosW);

    // ��ʼ��Ϊ0 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;


    ComputeDirectionalLight(g_Material, g_DirLight, pIn.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    

    float4 litColor = texColor * (ambient + diffuse) + spec;

    litColor.a = texColor.a * g_Material.Diffuse.a;
   // float4 litColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
  //  return float4(1.0f, 0.0f, 0.0f, 1.0f);
    return litColor;
}