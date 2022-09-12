#pragma once

#include"FDx11.h"
#include<map>
#include"Factory.h"

enum RasterizeStateType
{
	RS_NO_CULL,
	RS_WIREFRAME,
	RS_CULL_CLOCKWISE
};
enum SamplerStateType 
{
	ANISTROPIC_WRAP,
	LINIEAR_WRAP
};
enum BlendStateType 
{
	ALPHA_TO_COVERAGE,
	NO_COLOR_WRITE,
	TRANSPORT,
	ADDITIVE
};
enum DepthStencilStateType : uint8_t
{
	LESS_EQUAL,
	WRITE_STENCIL,
	DRAW_WITH_STENCIL,
	NO_DOUBLE_BLEND,
	NO_DEPTH_TEST,
	NO_DEPTH_WRITE,
	NO_DEPTH_TEST_WITH_STENCIL,
	NO_DEPTH_WRITE_WITH_STENCIL
};

class SamplerResoucePool : public ResourceFactory<ComPtr<ID3D11SamplerState>, SamplerStateType, SamplerResoucePool>
{
public:
	bool Init(const FDx11Device& _device)
	{
		ComPtr<ID3D11SamplerState> samplerLinearWrap;

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));

		// 线性过滤模式
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HR(_device.GetDevice()->CreateSamplerState(&sampDesc, samplerLinearWrap.GetAddressOf()));


		this->CreateResouce(SamplerStateType::LINIEAR_WRAP, samplerLinearWrap);

		ComPtr<ID3D11SamplerState> samplerAnistropicWrap;

		// 各向异性过滤模式
		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MaxAnisotropy = 4;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HR(_device.GetDevice()->CreateSamplerState(&sampDesc, samplerAnistropicWrap.GetAddressOf()));
		this->CreateResouce(SamplerStateType::ANISTROPIC_WRAP, samplerAnistropicWrap);
		return true;
	}
};

class BlendStateResoucePool : public ResourceFactory<ComPtr<ID3D11BlendState>, BlendStateType, BlendStateResoucePool>
{
public:
	bool Init(const FDx11Device& _device)
	{
		ComPtr<ID3D11BlendState> BSAlphaToCoverage = nullptr;
		ComPtr<ID3D11BlendState> BSNoColorWrite = nullptr;
		ComPtr<ID3D11BlendState> BSTransparent = nullptr;
		ComPtr<ID3D11BlendState> BSAdditive = nullptr;

		
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		auto& rtDesc = blendDesc.RenderTarget[0];
		// Alpha-To-Coverage模式
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		rtDesc.BlendEnable = false;
		rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HR(_device.GetDevice()->CreateBlendState(&blendDesc, BSAlphaToCoverage.GetAddressOf()));

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		rtDesc.BlendEnable = true;
		rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		HR(_device.GetDevice()->CreateBlendState(&blendDesc, BSTransparent.GetAddressOf()));
		rtDesc.SrcBlend = D3D11_BLEND_ONE;
		rtDesc.DestBlend = D3D11_BLEND_ONE;
		rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

		HR(_device.GetDevice()->CreateBlendState(&blendDesc, BSAdditive.GetAddressOf()));

		// 无颜色写入混合模式
		// Color = DestColor
		// Alpha = DestAlpha
		rtDesc.BlendEnable = false;
		rtDesc.SrcBlend = D3D11_BLEND_ZERO;
		rtDesc.DestBlend = D3D11_BLEND_ONE;
		rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
		rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;
		rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtDesc.RenderTargetWriteMask = 0;
		HR(_device.GetDevice()->CreateBlendState(&blendDesc, BSNoColorWrite.GetAddressOf()));

		this->CreateResouce(BlendStateType::ADDITIVE, BSAdditive);
		this->CreateResouce(BlendStateType::NO_COLOR_WRITE, BSNoColorWrite);
		this->CreateResouce(BlendStateType::TRANSPORT, BSTransparent);
		this->CreateResouce(BlendStateType::ALPHA_TO_COVERAGE, BSAlphaToCoverage);
		return true;

	}
	
};
class RasterStateResoucePool : public ResourceFactory<ComPtr<ID3D11RasterizerState>, RasterizeStateType, RasterStateResoucePool>
{
public:
	bool Init(const FDx11Device& _device)
	{
		ComPtr<ID3D11RasterizerState> RSWireframe = nullptr;
		ComPtr<ID3D11RasterizerState> RSNoCull = nullptr;
		ComPtr<ID3D11RasterizerState> RSCullClockWise = nullptr;

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

		// 线框模式
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		HR(_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));

		
		// 无背面剔除模式
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		HR(_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));

		// 顺时针剔除模式
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = true;
		rasterizerDesc.DepthClipEnable = true;
		HR(_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, RSCullClockWise.GetAddressOf()));

		this->CreateResouce(RasterizeStateType::RS_WIREFRAME, RSWireframe);
		this->CreateResouce(RasterizeStateType::RS_NO_CULL, RSNoCull);
		this->CreateResouce(RasterizeStateType::RS_CULL_CLOCKWISE, RSCullClockWise);
		return true;
	}
		
};

class DepthStencilStateResoucePool : public ResourceFactory<ComPtr<ID3D11DepthStencilState>, DepthStencilStateType, DepthStencilStateResoucePool>
{
public:
	bool Init(const FDx11Device& _device)
	{
		ComPtr<ID3D11DepthStencilState> DSSLessEqual = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSWriteStencil = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSDrawWithStencil = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSNoDoubleBlend = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSNoDepthTest = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSNoDepthWrite = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSNoDepthTestWithStencil = nullptr;
		ComPtr<ID3D11DepthStencilState> DSSNoDepthWriteWithStencil = nullptr;

		D3D11_DEPTH_STENCIL_DESC dsDesc;

		// 允许使用深度值一致的像素进行替换的深度/模板状态
		// 该状态用于绘制天空盒，因为深度值为1.0时默认无法通过深度测试
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf()));

		// 写入模板值的深度/模板状态
		// 这里不写入深度信息
		// 无论是正面还是背面，原来指定的区域的模板值都会被写入StencilRef
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));

		// 对指定模板值进行绘制的深度/模板状态
		// 对满足模板值条件的区域才进行绘制，并更新深度
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSDrawWithStencil.GetAddressOf()));

		// 无二次混合深度/模板状态
		// 允许默认深度测试
		// 通过自递增使得原来StencilRef的值只能使用一次，实现仅一次混合
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDoubleBlend.GetAddressOf()));

		// 关闭深度测试的深度/模板状态
		// 若绘制非透明物体，务必严格按照绘制顺序
		// 绘制透明物体则不需要担心绘制顺序
		// 而默认情况下模板测试就是关闭的
		dsDesc.DepthEnable = false;
		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthTest.GetAddressOf()));


		// 关闭深度测试
		// 若绘制非透明物体，务必严格按照绘制顺序
		// 绘制透明物体则不需要担心绘制顺序
		// 对满足模板值条件的区域才进行绘制
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthTestWithStencil.GetAddressOf()));

		// 进行深度测试，但不写入深度值的状态
		// 若绘制非透明物体时，应使用默认状态
		// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
		// 并且确保较前的非透明物体可以阻挡较后的一切物体
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthWrite.GetAddressOf()));


		// 进行深度测试，但不写入深度值的状态
		// 若绘制非透明物体时，应使用默认状态
		// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
		// 并且确保较前的非透明物体可以阻挡较后的一切物体
		// 对满足模板值条件的区域才进行绘制
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthWriteWithStencil.GetAddressOf()));
		
		this->CreateResouce(DepthStencilStateType::LESS_EQUAL, DSSLessEqual);
		this->CreateResouce(DepthStencilStateType::WRITE_STENCIL, DSSWriteStencil);
		this->CreateResouce(DepthStencilStateType::DRAW_WITH_STENCIL, DSSDrawWithStencil);
		this->CreateResouce(DepthStencilStateType::NO_DOUBLE_BLEND, DSSNoDoubleBlend);
		this->CreateResouce(DepthStencilStateType::NO_DEPTH_TEST, DSSNoDepthTest);
		this->CreateResouce(DepthStencilStateType::NO_DEPTH_WRITE, DSSNoDepthWrite);
		this->CreateResouce(DepthStencilStateType::NO_DEPTH_TEST_WITH_STENCIL, DSSNoDepthTestWithStencil);
		this->CreateResouce(DepthStencilStateType::NO_DEPTH_WRITE_WITH_STENCIL, DSSNoDepthWriteWithStencil);
		return true;
	}
};
