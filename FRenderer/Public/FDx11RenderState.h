#pragma once

#include"FDx11.h"
#include<map>

//enum RasterizeStateType
//{
//	RS_NO_CULL,
//	RS_WIREFRAME,
//	RS_CULL_CLOCKWISE
//};
//enum SamplerStateType 
//{
//	ANISTROPIC_WRAP,
//	LINIEAR_WRAP
//};
//enum BlendStateType 
//{
//	ALPHA_TO_COVERAGE,
//	NO_COLOR_WRITE,
//	TRANSPORT,
//	ADDITIVE
//};
//enum DepthStencilStateType : uint8_t
//{
//	LESS_EQUAL,
//	WRITE_STENCIL,
//	DRAW_WITH_STENCIL,
//	NO_DOUBLE_BLEND,
//	NO_DEPTH_TEST,
//	NO_DEPTH_WRITE,
//	NO_DEPTH_TEST_WITH_STENCIL,
//	NO_DEPTH_WRITE_WITH_STENCIL
//};

//typedef struct RenderStateSet
//{
//	ComPtr<ID3D11RasterizerState> mRasterizeState;
//	ComPtr<ID3D11SamplerState> mSamplerState;
//	ComPtr<ID3D11BlendState> mBlendState;
//	ComPtr<ID3D11DepthStencilState> mDepthStencilState;
//}RenderStateSet;

//class RenderStateFactory final
//{
//public:
//	RenderStateFactory() = delete;
//
//	static ComPtr<ID3D11DepthStencilState> GetDepthStencilState(DepthStencilStateType type)
//	{
//		if (mDepthStencilStateSet.find(type) != mDepthStencilStateSet.end())
//		{
//			return mDepthStencilStateSet.at(type);
//		}
//		return ComPtr<ID3D11DepthStencilState>(nullptr);
//	}
//	static ComPtr<ID3D11RasterizerState> GetRasterizeStateType(RasterizeStateType type)
//	{
//		if (mRasterizerStateSet.find(type) != mRasterizerStateSet.end())
//		{
//			return mRasterizerStateSet.at(type);
//		}
//		return ComPtr<ID3D11RasterizerState>(nullptr);
//	}
//	static ComPtr<ID3D11BlendState> GetBlendState(BlendStateType type)
//	{
//		if (mBlendStateSet.find(type) != mBlendStateSet.end())
//		{
//			return mBlendStateSet.at(type);
//		}
//		return ComPtr<ID3D11BlendState>(nullptr);
//	}
//	static ComPtr<ID3D11SamplerState> GetSamplerState(SamplerStateType type)
//	{
//		if (mSamplerStateSet.find(type) != mSamplerStateSet.end())
//		{
//			return mSamplerStateSet.at(type);
//		}
//		return ComPtr<ID3D11SamplerState>(nullptr);
//	}
//	static void RegisterAllState(ID3D11Device* device)
//	{
//		D3D11_RASTERIZER_DESC rasterizerDesc;
//		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
//
//		// Ïß¿òÄ£Ê½
//		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
//		rasterizerDesc.CullMode = D3D11_CULL_NONE;
//		rasterizerDesc.FrontCounterClockwise = false;
//		rasterizerDesc.DepthClipEnable = true;
//		ComPtr<ID3D11RasterizerState> RSWireframe = nullptr;
//		HR(device->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));
//		mRasterizerStateSet[RasterizeStateType::RS_NO_CULL] = RSWireframe;
//		
//	}
//private:
//	
//	static std::unordered_map< RasterizeStateType, ComPtr<ID3D11RasterizerState>> mRasterizerStateSet;
//	static std::unordered_map< SamplerStateType, ComPtr<ID3D11SamplerState>> mSamplerStateSet;
//	static std::unordered_map< DepthStencilStateType, ComPtr<ID3D11DepthStencilState>> mDepthStencilStateSet;
//	static std::unordered_map< BlendStateType, ComPtr<ID3D11BlendState>> mBlendStateSet;
//
//};