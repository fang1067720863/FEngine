#pragma once
#include<map>

#include"FDx11InpuyLayout.h"
#include"FDx11BufferObject.h"
#include"FDx11Util.h"
#include"UtfConverter.h"


class ShaderVariable
{

};

enum ShaderType : uint16_t
{
	ST_Vertex,
	ST_Pixel,
};
const D3D11_INPUT_ELEMENT_DESC VertexInputElement[1] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//,{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
const D3D11_INPUT_ELEMENT_DESC PNT_InputElement[3] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC vertexDesc_disjoint[3] =
{
  {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,      0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
  {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,   1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
  {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,         2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
};


class FDx11GpuProgram:public FReference {
public:
	typedef Ptr<ConstantBufferObject> ConstantBufferObjectPtr;
	typedef std::vector<ConstantBufferObjectPtr> CboArray;
	typedef ComPtr <ID3D11ShaderResourceView> GpuTextureView;
	typedef std::vector <GpuTextureView> GpuTextureViewArray;
	typedef ComPtr <ID3D11SamplerState> GpuSamplerView;
	typedef std::vector <GpuSamplerView> GpuSamplerViewArray;

	FDx11GpuProgram(const FDx11Device& _device) :device(_device) { Init(); }
	ComPtr<ID3D11InputLayout> inputLayout;
	bool Init()
	{
		std::string shaderPath = GLOBAL_PATH + "Shader//";
		vsFileName = "DefaultVertex";
		psFileName = "DefaultPixel";
		const std::string hlslExt = ".hlsl";
		const std::string csoExt = ".cso";

		//Triangle_VS
		HR(CreateShaderFromFile(ConvertUtf(shaderPath + vsFileName + csoExt).c_str(), ConvertUtf(shaderPath + vsFileName + hlslExt).c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device.GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
		// 创建顶点布局
		HR(device.GetDevice()->CreateInputLayout(PNT_InputElement, ARRAYSIZE(PNT_InputElement), blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf()));
		//HR(device.GetDevice()->CreateInputLayout(PNT_InputElement, ARRAYSIZE(PNT_InputElement), blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf()));
		HR(CreateShaderFromFile(ConvertUtf(shaderPath + psFileName + csoExt).c_str(), ConvertUtf(shaderPath + psFileName + hlslExt).c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device.GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));
		return true;
	}

	bool UseProgram()
	{
		device.GetDeviceContext()->VSSetShader(GetVertexShader(), nullptr, 0);
		device.GetDeviceContext()->PSSetShader(GetPixelShader(), nullptr, 0);

		// 一个slot 只设置一块resource element ,不设置成Array magicNumber = 1
		// shader resource constant buffer
		unsigned int num = GetConstantBufferNum();
		for (unsigned int i = 0; i < num; i++)
		{
			ID3D11Buffer* bufferArray[1];
			ConstantBufferObject* cbo = GetContantBufferObject(i);
			bufferArray[0] = cbo->GetBufferView();

			UINT slot = i;
			device.GetDeviceContext()->VSSetConstantBuffers(slot, 1, bufferArray);
			device.GetDeviceContext()->PSSetConstantBuffers(slot, 1, bufferArray);
		}

		// shader resource texture
		num = GetTextureNum();
		for (unsigned int i = 0; i < num; i++)
		{

			UINT slot = i;
			UINT num = 1;
			device.GetDeviceContext()->PSSetShaderResources(slot, num, GetGpuTextureView(i));
		}

		// shader resource sampler
		num = GetSamplerNum();
		for (unsigned int i = 0; i < num; i++)
		{
			UINT slot = i;
			UINT num = 1;
			device.GetDeviceContext()->PSSetSamplers(slot, num, GetSamplerView(i));
		}
		return true;
	}
	
	bool AddConstantBuffer(ConstantBufferObjectPtr cbo)
	{
		mCboList.push_back(cbo);
		return true;
	}

	bool AddTextureResource(GpuTextureView texturePtr)
	{
		mTextureArrayView.push_back(texturePtr); 
		return true;
		//emplace_back std::move(smartPtr) ? 
	}
	bool AddSamplerResource(GpuSamplerView samplerPtr)
	{
		mSamplerArrayView.push_back(samplerPtr);
		return true;
	}
	
	ID3DBlob* GetD3DBlob() { return blob.Get(); }


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
		return mCboList.size();
	}

	UINT GetTextureNum() const
	{
		return mTextureArrayView.size();
	}

	ID3D11ShaderResourceView** GetGpuTextureView(unsigned int i)
	{
		return mTextureArrayView[i].GetAddressOf();
    }

	UINT GetSamplerNum() const
	{
		return mSamplerArrayView.size();
	}

	ID3D11SamplerState** GetSamplerView(unsigned int i)
	{
		return mSamplerArrayView[i].GetAddressOf();
	}

protected:

	// definition
	ComPtr<ID3D11VertexShader> mVertexShader;
	ComPtr<ID3D11PixelShader> mPixelShader;
	D3D11_SHADER_VARIABLE_DESC desc;
	std::string vsFileName;
	std::string psFileName;
	std::string shaderPath;

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

	//ID3D11Device* device; 
	const FDx11Device& device;
};