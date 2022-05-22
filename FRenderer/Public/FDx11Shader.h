#pragma once
#include<map>
#include"FDx11.h"
#include"FDx11InpuyLayout.h"

HRESULT CreateShaderFromFile(
	const WCHAR* csoFileNameInOut,
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// 寻找是否有已经编译好的顶点着色器
	//if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
	//{
	//	return hr;
	//}
	//else

//	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//	// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
//	// 但仍然允许着色器进行优化操作
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//
//	// 在Debug环境下禁用优化以避免出现一些不合理的情况
//	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//	ID3DBlob* errorBlob = nullptr;
//
//	hr = D3DX11CompileFromFile(hlslFileName, nullptr, nullptr, entryPoint, shaderModel,
//		dwShaderFlags, 0, nullptr, ppBlobOut, &errorBlob, nullptr);
//
//	if (FAILED(hr))
//	{
//		if (errorBlob != nullptr)
//		{
//			const char* errstr = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
//			std::cout << errstr << std::endl;
//		}
//		//SAFE_RELEASE(errorBlob);
//		return hr;
//	}
//
//	// 若指定了输出文件名，则将着色器二进制信息输出
//	if (csoFileNameInOut)
//	{
//
//		//return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
//	}


	return hr;
}

class ShaderVariable
{

};

enum ShaderType : uint16_t
{
	Vertex,
	Pixel,
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
	typedef std::shared_ptr<ConstantBufferObject> ConstantBufferObjectPtr;
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
};