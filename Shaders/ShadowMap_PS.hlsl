//Texture2D g_DiffuseMap : register(t0);
//SamplerState g_Sam : register(s0);

struct VertexPosHTex
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

// ���������Alpha���βü����Ա�֤��Ӱ����ʾ��ȷ��
// ���ڲ���Ҫ����������������ļ��������ֱ�ӽ�����
// ��ɫ����Ϊnullptr
void PS(VertexPosHTex pIn)
{
    //float4 diffuse = g_DiffuseMap.Sample(g_Sam, pIn.Tex);

    //// ��Ҫ��͸������д�������ͼ
    //clip(diffuse.a - 0.1f);
}














//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}