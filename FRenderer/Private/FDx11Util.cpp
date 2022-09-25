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

	// Ѱ���Ƿ����Ѿ�����õĶ�����ɫ��
	//if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
	//{
	//	return hr;
	//}
	//else

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// ���� D3DCOMPILE_DEBUG ��־���ڻ�ȡ��ɫ��������Ϣ���ñ�־���������������飬
	// ����Ȼ������ɫ�������Ż�����
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// ��Debug�����½����Ż��Ա������һЩ����������
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

	// ��ָ��������ļ���������ɫ����������Ϣ���
	if (csoFileNameInOut)
	{

		//return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
	}


	return hr;
}