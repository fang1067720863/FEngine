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
// 成员内部变量  mDx11Buffer  一般生命周期和当前实例保持一致
// 成员内部变量  拿来使用 小写不加m  如 device
// 外放成员函数 成员函数GetElementFormat() 
// internel 成员函数  _createShaderResorce 小写 下划线开头
// 枚举成员 全大写 下划线     eg: TEXTURE_2D_ARRAY

struct FDx11Device
{
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;

	inline ID3D11Device* GetDevice() const { return device.Get(); }
	inline ID3D11DeviceContext* GetDeviceContext()const { return deviceContext.Get(); }
	inline ID3D11DeviceContext** GetDeviceContextAddress() { return deviceContext.GetAddressOf(); }
	inline ID3D11Device** GetDeviceAddress() { return device.GetAddressOf(); }
};

// Debug模式下的错误提醒与追踪
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