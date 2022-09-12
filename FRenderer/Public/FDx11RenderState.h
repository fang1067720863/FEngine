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

		// ���Թ���ģʽ
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

		// �������Թ���ģʽ
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
		// Alpha-To-Coverageģʽ
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

		// ����ɫд����ģʽ
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

		// �߿�ģʽ
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		HR(_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));

		
		// �ޱ����޳�ģʽ
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		HR(_device.GetDevice()->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));

		// ˳ʱ���޳�ģʽ
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

		// ����ʹ�����ֵһ�µ����ؽ����滻�����/ģ��״̬
		// ��״̬���ڻ�����պУ���Ϊ���ֵΪ1.0ʱĬ���޷�ͨ����Ȳ���
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf()));

		// д��ģ��ֵ�����/ģ��״̬
		// ���ﲻд�������Ϣ
		// ���������滹�Ǳ��棬ԭ��ָ���������ģ��ֵ���ᱻд��StencilRef
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
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));

		// ��ָ��ģ��ֵ���л��Ƶ����/ģ��״̬
		// ������ģ��ֵ����������Ž��л��ƣ����������
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
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSDrawWithStencil.GetAddressOf()));

		// �޶��λ�����/ģ��״̬
		// ����Ĭ����Ȳ���
		// ͨ���Ե���ʹ��ԭ��StencilRef��ֵֻ��ʹ��һ�Σ�ʵ�ֽ�һ�λ��
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
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDoubleBlend.GetAddressOf()));

		// �ر���Ȳ��Ե����/ģ��״̬
		// �����Ʒ�͸�����壬����ϸ��ջ���˳��
		// ����͸����������Ҫ���Ļ���˳��
		// ��Ĭ�������ģ����Ծ��ǹرյ�
		dsDesc.DepthEnable = false;
		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthTest.GetAddressOf()));


		// �ر���Ȳ���
		// �����Ʒ�͸�����壬����ϸ��ջ���˳��
		// ����͸����������Ҫ���Ļ���˳��
		// ������ģ��ֵ����������Ž��л���
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthTestWithStencil.GetAddressOf()));

		// ������Ȳ��ԣ�����д�����ֵ��״̬
		// �����Ʒ�͸������ʱ��Ӧʹ��Ĭ��״̬
		// ����͸������ʱ��ʹ�ø�״̬������Чȷ�����״̬�Ľ���
		// ����ȷ����ǰ�ķ�͸����������赲�Ϻ��һ������
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = false;

		HR(_device.GetDevice()->CreateDepthStencilState(&dsDesc, DSSNoDepthWrite.GetAddressOf()));


		// ������Ȳ��ԣ�����д�����ֵ��״̬
		// �����Ʒ�͸������ʱ��Ӧʹ��Ĭ��״̬
		// ����͸������ʱ��ʹ�ø�״̬������Чȷ�����״̬�Ľ���
		// ����ȷ����ǰ�ķ�͸����������赲�Ϻ��һ������
		// ������ģ��ֵ����������Ž��л���
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
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
