//#include"FDx11InputLayout.h"
#include"../Public/FDx11InpuyLayout.h"
#include"../Public/FDx11Shader.h"

const D3D11_INPUT_ELEMENT_DESC VertexPosNormalTex[3] = {
   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
void FDx11VertexInputLayout::PrepareDxResource(FDx11GpuProgram* program)
{
    //inputLayoutDescs.data(), mElementNum,
   
    HR(device.GetDevice()->CreateInputLayout(VertexPosNormalTex, ARRAYSIZE(VertexPosNormalTex), program->GetD3DBlob()->GetBufferPointer(), program->GetD3DBlob()->GetBufferSize(), inputLayout.GetAddressOf()));
}