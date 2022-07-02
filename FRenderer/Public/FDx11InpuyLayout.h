#pragma once


#include"FDx11.h"
#include"Reference.h"
#include "CommonType.h"
//#include"FDx11Shader.h"

#include<map>

struct VertexElementDesc
{
    std::string sematic;
    DXGI_FORMAT format;
    uint32_t byteSize;
};

const std::map<VertexElementType, VertexElementDesc> VertexElementDescMap = {
    {VertexElementType::POSITION, {"POSITION",DXGI_FORMAT_R32G32B32_FLOAT,12}},
    {VertexElementType::NORMAL, {"NORMAL",DXGI_FORMAT_R32G32B32_FLOAT,12}},
    {VertexElementType::TEXCORD0, {"TEXCORD0",DXGI_FORMAT_R32G32_FLOAT,8}}
};

class FDx11GpuProgram;
class FDx11VertexInputLayout:public FReference{

public:  
    FDx11VertexInputLayout(VertexElementType type, FDx11GpuProgram* program,  const FDx11Device& _device):mVertexElementType(type),device(_device)
    {
        GenVertexInputDesc(type);
        PrepareDxResource(program);
    }

    ~FDx11VertexInputLayout() {
        free(inputLayoutDescs);
    }

    void GenVertexInputDesc(VertexElementType type)
    {
        for (uint32_t i = 0; i < VertexElementType::BASIC_NUM; i++)
        {
            if (((1 << i) & type) != 0)
            {
                mElementNum++;
            }
        }
        inputLayoutDescs = (D3D11_INPUT_ELEMENT_DESC*)malloc(inputElementSize * mElementNum);

        unsigned int vOffset = 0;
        for (uint32_t i = 0; i < VertexElementType::BASIC_NUM; i++)
        {
            if (((1 << i) & type) != 0)
            {

                D3D11_INPUT_ELEMENT_DESC& desc = inputLayoutDescs[mElementNum];
                VertexElementDesc veDesc = VertexElementDescMap.at((VertexElementType)i);
                desc.SemanticName = veDesc.sematic.c_str();
                desc.SemanticIndex = 0;
                desc.Format = veDesc.format;
                desc.InputSlot = 0;
                desc.AlignedByteOffset = vOffset;
                vOffset += veDesc.byteSize;
                desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                desc.InstanceDataStepRate = 0;
            }
        }
       
  
    }

    void PrepareDxResource(FDx11GpuProgram* program);
    
    unsigned int GetElementNum() const { return mElementNum; }
 
    const D3D11_INPUT_ELEMENT_DESC* GetInputLayoutDesc()const { return inputLayoutDescs; }

    ID3D11InputLayout* GetD3D11InputLayout() {
        return inputLayout.Get();
    }

    ID3D11InputLayout** GetD3D11InputLayoutAddress() {
        return inputLayout.GetAddressOf();
    }
    D3D11_INPUT_ELEMENT_DESC* GetinputLayoutDescs() const { return inputLayoutDescs; }

private:
    const FDx11Device& device;
    D3D11_INPUT_ELEMENT_DESC* inputLayoutDescs;
    ComPtr<ID3D11InputLayout> inputLayout;
    VertexElementType mVertexElementType;
    unsigned int mElementNum = 0;

    static const int  inputElementSize = sizeof(D3D11_INPUT_ELEMENT_DESC);
    const int max_elelment_num = 5;
    
};