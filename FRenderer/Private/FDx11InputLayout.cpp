//#include"FDx11InputLayout.h"
#include"../Public/FDx11InpuyLayout.h"
#include"../Public/FDx11Shader.h"

void FDx11VertexInputLayout::PrepareDxResource(FDx11GpuProgram* program)
{
    device.GetDevice()->CreateInputLayout(inputLayoutDescs, mElementNum, program->GetD3DBlob()->GetBufferPointer(), program->GetD3DBlob()->GetBufferSize(), inputLayout.GetAddressOf());
}