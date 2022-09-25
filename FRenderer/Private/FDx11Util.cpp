#include"FDx11Util.h"

#include"FDx11.h"
#include <d3dcompiler.h>
#include <d3dx11.h>
#include<d3d11.h>
#include<string>
#include<string_view>
#include<iostream>



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

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
	// 但仍然允许着色器进行优化操作
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// 在Debug环境下禁用优化以避免出现一些不合理的情况
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* errorBlob = nullptr;

	hr = D3DX11CompileFromFile(hlslFileName, nullptr, nullptr, entryPoint, shaderModel,
		dwShaderFlags, 0, nullptr, ppBlobOut, &errorBlob, nullptr);

	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
		{
			const char* errstr = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			std::cout << errstr << std::endl;
		}
		//SAFE_RELEASE(errorBlob);
		return hr;
	}

	// 若指定了输出文件名，则将着色器二进制信息输出
	if (csoFileNameInOut)
	{

		//return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
	}


	return hr;
}