//
//cbuffer CBObject : register(b0)
//{
//    matrix g_World;
//}
//cbuffer CBStatic : register(b1)
//{
//    matrix g_ViewProj;
//}
//
//
//struct VertexPosNormalTex
//{
//    float3 PosL : POSITION;
//    float3 NormalL : NORMAL;
//    float2 Tex : TEXCOORD;
//};
//
//struct VertexPosHTex
//{
//    float4 PosH : SV_POSITION;
//    float2 Tex : TEXCOORD;
//};
//
//VertexPosHTex VS(VertexPosNormalTex vIn)
//{
//    VertexPosHTex vOut;
//
//    vOut.PosH = mul(float4(vIn.PosL, 1.0f), g_World);
//    vOut.PosH = mul(vOut.PosH,  g_ViewProj);
//    vOut.Tex = vIn.Tex;
//
//    return vOut;
//}
//
//
////float4 main( float4 pos : POSITION ) : SV_POSITION
////{
////	return pos;
////}