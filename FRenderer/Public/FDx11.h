#pragma once


#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include<D3Dcompiler.h>
#include<DXGIFormat.h>


#include<string>
#include<memory>
#include<vector>
#include<assert.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
// ��Ա�ڲ�����  mDx11Buffer  һ���������ں͵�ǰʵ������һ��
// ��Ա�ڲ�����  ����ʹ�� Сд����m  �� device
// ��ų�Ա���� ��Ա����GetElementFormat() 
// internel ��Ա����  _createShaderResorce Сд �»��߿�ͷ
// ö�ٳ�Ա ȫ��д �»���     eg: TEXTURE_2D_ARRAY

struct FDx11Device
{
	ComPtr<ID3D11Device> device = nullptr;
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

	inline ID3D11Device* GetDevice() const { return device.Get(); }
	inline ID3D11DeviceContext* GetDeviceContext()const { return deviceContext.Get(); }
	inline ID3D11DeviceContext** GetDeviceContextAddress() { return deviceContext.GetAddressOf(); }
	inline ID3D11Device** GetDeviceAddress() { return device.GetAddressOf(); }
};
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ std::string_view name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.data());
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}
// Debugģʽ�µĴ���������׷��
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr,
	_In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox); 

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
			{															\
				HRESULT hr = (x);										\
				if(FAILED(hr))											\
				{														\
					DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
				}														\
			}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 

#endif