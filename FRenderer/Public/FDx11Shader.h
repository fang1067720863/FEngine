#pragma once
#include<map>

#include"FDx11InpuyLayout.h"
#include"FDx11BufferObject.h"
#include"FDx11Util.h"
#include"UtfConverter.h"
#include"Constant.h"
#include"FDx11ResourceFactory.h"
#include"FDx11RenderState.h"



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

	FDx11GpuProgram(const FDx11Device& _device,const std::string& _vsName, const std::string& _psName )
		:device(_device), vsFileName(_vsName), psFileName(_psName) { Init(); }
	ComPtr<ID3D11InputLayout> inputLayout;

	bool Init()
	{
		std::string shaderPath = GLOBAL_PATH + "Shaders//";
		const std::string hlslExt = ".hlsl";
		const std::string csoExt = ".cso";

		//Triangle_VS
		HR(CreateShaderFromFile(ConvertUtf(shaderPath + vsFileName + csoExt).c_str(), ConvertUtf(shaderPath + vsFileName + hlslExt).c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device.GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
		// 创建顶点布局
		HR(device.GetDevice()->CreateInputLayout(PNT_InputElement, ARRAYSIZE(PNT_InputElement), blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf()));
		HR(CreateShaderFromFile(ConvertUtf(shaderPath + psFileName + csoExt).c_str(), ConvertUtf(shaderPath + psFileName + hlslExt).c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
		HR(device.GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));
		return true;
	}


	bool UseProgram()
	{
		device.GetDeviceContext()->VSSetShader(GetVertexShader(), nullptr, 0);
		device.GetDeviceContext()->PSSetShader(GetPixelShader(), nullptr, 0);


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

		for (auto textureSlot : mMaterialSlot.textureSlots)
		{
			auto texture = ShaderResoucePool::Instance().GetResource(textureSlot.resourceSlot);
			ID3D11ShaderResourceView* srArray[1];
			srArray[0] = texture.Get();
			device.GetDeviceContext()->PSSetShaderResources(textureSlot.slotId, textureSlot.num, srArray);
		}
		for (auto samplerSlot : mMaterialSlot.samplerSlots)
		{
			auto sampler = SamplerResoucePool::Instance().GetResource(SamplerStateType::LINIEAR_WRAP);
			ID3D11SamplerState* samplerArray[1];
			samplerArray[0] = sampler.Get();
			device.GetDeviceContext()->PSSetSamplers(0, samplerSlot.num, samplerArray);
		}
		return true;
	}
	
	bool AddConstantBuffer(ConstantBufferObjectPtr cbo)
	{
		mCboList.push_back(cbo);
		return true;
	}

	int AddShaderResource(const std::string& texturePath, int32_t shaderSlot)
	{
		int32_t resourceSlot = ShaderResoucePool::Instance().CreateDeviceResource(texturePath, device);
		return AddShaderResource(resourceSlot, shaderSlot);
	}

	int AddShaderResource(int32_t resourceSlot, int32_t shaderSlot)
	{
		mMaterialSlot.textureSlots.push_back(ShaderInputSlot::TextureSlot{ shaderSlot, 1, resourceSlot });
		return resourceSlot;
	}

	void AddSamplerResource(SamplerStateType type = SamplerStateType::LINIEAR_WRAP, int32_t shaderSlot = 0)
	{
		//int32_t resourceSlot = SamplerResoucePool::Instance().GetResource(type);
		mMaterialSlot.samplerSlots.push_back(ShaderInputSlot::SamplerSlot{ shaderSlot, 1, 1 });
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
	//UINT mTextureNum;
	//GpuTextureViewArray mTextureArrayView;

	//UINT mSamplerNum;
	//GpuSamplerViewArray mSamplerArrayView;
	ShaderInputSlot mMaterialSlot;

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