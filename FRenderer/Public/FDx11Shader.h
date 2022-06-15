#pragma once
#include<map>

#include"FDx11InpuyLayout.h"
#include"FDx11BufferObject.h"
#include"FDx11Util.h"


class ShaderVariable
{

};

enum ShaderType : uint16_t
{
	ST_Vertex,
	ST_Pixel,
};

class FDx11GpuProgram {
public:

	bool Init()
	{
		HR(CreateShaderFromFile(L"HLSL\\DefaultVertex.cso", L"HLSL\\DefaultVertex.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
		// 创建顶点布局
		//device->CreateInputLayout(mInputLayout->GetInputLayoutDesc(), mInputLayout->GetElementNum(), blob->GetBufferPointer(), blob->GetBufferSize(), mInputLayout->GetD3D11InputLayoutAddress());

		HR(CreateShaderFromFile(L"HLSL\\DefaultPixel.cso", L"HLSL\\DefaultPixel.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));
	}




	ID3D11VertexShader* GetVertexShader() const {
		return mVertexShader.Get();
	}
	ID3D11PixelShader* GetPixelShader() const {
		return mPixelShader.Get();
	}

	ID3D11ClassInstance** GetClassInstance(ShaderType type)
	{
		return mClassInstances[type];
	}
	UINT GetNumInstance(ShaderType type)
	{
		return mNumClassInstances[type];
	}

	ConstantBufferObject* GetContantBufferObject(unsigned int i)
	{
		return mCboList[i].get();
	}

	UINT GetConstantBufferNum () const
	{
		return mConstantBufferNum;
	}

	UINT GetTextureNum() const
	{
		return mTextureNum;
	}

	ID3D11ShaderResourceView** GetGpuTextureView(unsigned int i)
	{
		return mTextureArrayView[i].GetAddressOf();
    }

	UINT GetSamplerNum() const
	{
		return mSamplerNum;
	}

	ID3D11SamplerState** GetSamplerView(unsigned int i)
	{
		return mSamplerArrayView[i].GetAddressOf();
	}

protected:
	typedef Ptr<ConstantBufferObject> ConstantBufferObjectPtr;
	typedef std::vector<ConstantBufferObjectPtr> CboArray;
	typedef ComPtr <ID3D11ShaderResourceView> GpuTextureView;
	typedef std::vector <GpuTextureView> GpuTextureViewArray;
	typedef ComPtr <ID3D11SamplerState> GpuSamplerView;
	typedef std::vector <GpuSamplerView> GpuSamplerViewArray;

	// definition
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	D3D11_SHADER_VARIABLE_DESC desc;
	std::string vsFileName;
	std::string psFileName;

	// shader resource 
	UINT mConstantBufferNum;
	CboArray mCboList;
	UINT mNumSlots;
	//
	UINT mTextureNum;
	GpuTextureViewArray mTextureArrayView;

	UINT mSamplerNum;
	GpuSamplerViewArray mSamplerArrayView;


	ComPtr<ID3DBlob> blob;

	// List of class instances per shader stage
	ID3D11ClassInstance* mClassInstances[6][8];

	// Number of class instances per shader stage
	UINT mNumClassInstances[6];

	// Store created shader subroutines, to prevent creation and destruction every frame
	typedef std::map<std::string, ID3D11ClassInstance*> ClassInstanceMap;
	typedef std::map<std::string, ID3D11ClassInstance*>::iterator ClassInstanceIterator;
	ClassInstanceMap mInstanceMap;

	ID3D11Device* device; 
};