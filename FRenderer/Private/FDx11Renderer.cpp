
#include"FDx11Renderer.h"
#include"FDx11RenderState.h"



ComPtr<ID3D11RasterizerState> RenderStates::RSNoCull = nullptr;
ComPtr<ID3D11RasterizerState> RenderStates::RSWireframe = nullptr;
ComPtr<ID3D11RasterizerState> RenderStates::RSCullClockWise = nullptr;

ComPtr<ID3D11SamplerState> RenderStates::SSAnistropicWrap = nullptr;
ComPtr<ID3D11SamplerState> RenderStates::SSLinearWrap = nullptr;

ComPtr<ID3D11BlendState> RenderStates::BSAlphaToCoverage = nullptr;
ComPtr<ID3D11BlendState> RenderStates::BSNoColorWrite = nullptr;
ComPtr<ID3D11BlendState> RenderStates::BSTransparent = nullptr;
ComPtr<ID3D11BlendState> RenderStates::BSAdditive = nullptr;

ComPtr<ID3D11DepthStencilState> RenderStates::DSSLessEqual = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSWriteStencil = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSDrawWithStencil = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDoubleBlend = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthTest = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthWrite = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthTestWithStencil = nullptr;
ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthWriteWithStencil = nullptr;

bool RenderStates::IsInit()
{
	return RSWireframe != nullptr;
}

void RenderStates::InitAll(ID3D11Device* device)
{
	// ��ǰ��ʼ�����Ļ���û��Ҫ������
	if (IsInit())
		return;
	// ******************
	// ��ʼ����դ����״̬
	//
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	// �߿�ģʽ
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));

	// �ޱ����޳�ģʽ
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));

	// ˳ʱ���޳�ģʽ
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&rasterizerDesc, RSCullClockWise.GetAddressOf()));

	// ******************
	// ��ʼ��������״̬
	//
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
	HR(device->CreateSamplerState(&sampDesc, SSLinearWrap.GetAddressOf()));

	// �������Թ���ģʽ
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device->CreateSamplerState(&sampDesc, SSAnistropicWrap.GetAddressOf()));

	// ******************
	// ��ʼ�����״̬
	//
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	auto& rtDesc = blendDesc.RenderTarget[0];
	// Alpha-To-Coverageģʽ
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	rtDesc.BlendEnable = false;
	rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(device->CreateBlendState(&blendDesc, BSAlphaToCoverage.GetAddressOf()));

	// ͸�����ģʽ
	// Color = SrcAlpha * SrcColor + (1 - SrcAlpha) * DestColor 
	// Alpha = SrcAlpha
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	rtDesc.BlendEnable = true;
	rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	HR(device->CreateBlendState(&blendDesc, BSTransparent.GetAddressOf()));

	// �ӷ����ģʽ
	// Color = SrcColor + DestColor
	// Alpha = SrcAlpha
	rtDesc.SrcBlend = D3D11_BLEND_ONE;
	rtDesc.DestBlend = D3D11_BLEND_ONE;
	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	HR(device->CreateBlendState(&blendDesc, BSAdditive.GetAddressOf()));

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
	HR(device->CreateBlendState(&blendDesc, BSNoColorWrite.GetAddressOf()));

	// ******************
	// ��ʼ�����/ģ��״̬
	//
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// ����ʹ�����ֵһ�µ����ؽ����滻�����/ģ��״̬
	// ��״̬���ڻ�����պУ���Ϊ���ֵΪ1.0ʱĬ���޷�ͨ����Ȳ���
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.StencilEnable = false;

	HR(device->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf()));

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

	HR(device->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));

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

	HR(device->CreateDepthStencilState(&dsDesc, DSSDrawWithStencil.GetAddressOf()));

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

	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDoubleBlend.GetAddressOf()));

	// �ر���Ȳ��Ե����/ģ��״̬
	// �����Ʒ�͸�����壬����ϸ��ջ���˳��
	// ����͸����������Ҫ���Ļ���˳��
	// ��Ĭ�������ģ����Ծ��ǹرյ�
	dsDesc.DepthEnable = false;
	dsDesc.StencilEnable = false;

	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthTest.GetAddressOf()));


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

	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthTestWithStencil.GetAddressOf()));

	// ������Ȳ��ԣ�����д�����ֵ��״̬
	// �����Ʒ�͸������ʱ��Ӧʹ��Ĭ��״̬
	// ����͸������ʱ��ʹ�ø�״̬������Чȷ�����״̬�Ľ���
	// ����ȷ����ǰ�ķ�͸����������赲�Ϻ��һ������
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;

	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthWrite.GetAddressOf()));


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

	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthWriteWithStencil.GetAddressOf()));

	// ******************
}


FDx11Renderer::FDx11Renderer(FGraphicContext::Traits* traits, HWND hwnd):mTraits(traits),
mHwnd(hwnd)
{
	
}


void FDx11Renderer::Initialize()
{
	InitDirect3D(mTraits, mHwnd);
	//InitSinglePass()
}


void FDx11Renderer::Update()
{

}

void FDx11Renderer::Draw()
{
	ExecutePass(singlePass.get());

}
void FDx11Renderer::InitSinglePass()
{
	singlePass = new FDx11Pass(1);

}


void FDx11Renderer::ExecutePass(FDx11Pass* gBufferPass)
{
	// Set Viewport
	mDeviceContext->RSSetViewports(gBufferPass->GetNumViews(), gBufferPass->GetViewport());
	mDeviceContext->RSSetScissorRects(1,gBufferPass->GetScissorRects());
	
	// Clear&Set FrameBuffer
	ClearFrameBuffer(gBufferPass);
	mDeviceContext->OMSetRenderTargets(gBufferPass->GetNumViews(), gBufferPass->GetRenderTargetViewAddress(), gBufferPass->GetDepthStencilView());

	// Render state
	//SetRenderStates(gBufferPass->GetRenderStates());
	SetRenderStates();
	// shader & shader resource
	SetGpuProgram(gBufferPass->GetGpuProgram());

	// Draw GameObject

}

void FDx11Renderer::SetGpuProgram(FDx11GpuProgram* program)
{
	// shader
	mDeviceContext->VSSetShader(program->GetVertexShader(), program->GetClassInstance(ShaderType::ST_Vertex),
		program->GetNumInstance(ShaderType::ST_Vertex));
	mDeviceContext->PSSetShader(program->GetPixelShader(), program->GetClassInstance(ShaderType::ST_Pixel),
		program->GetNumInstance(ShaderType::ST_Pixel));

	// һ��slot ֻ����һ��resource element ,�����ó�Array magicNumber = 1
	// shader resource constant buffer
	unsigned int num = program->GetConstantBufferNum();
	for (unsigned int i = 0; i < num; i++)
	{
		ID3D11Buffer* bufferArray[1];
		ConstantBufferObject* cbo = program->GetContantBufferObject(0);
		bufferArray[0] = cbo->GetBufferView();

		UINT slot = i;
		mDeviceContext->VSSetConstantBuffers(slot, 1, bufferArray);
	}

	// shader resource texture
	num = program->GetTextureNum();
	for (unsigned int i = 0; i < num; i++)
	{

		UINT slot = i;
		UINT num = 1;
		mDeviceContext->PSSetShaderResources(slot, num, program->GetGpuTextureView(i));
	}

	// shader resource sampler
	num = program->GetSamplerNum();
	for (unsigned int i = 0; i < num; i++)
	{
		UINT slot = i;
		UINT num = 1;
		mDeviceContext->PSSetSamplers(slot, num, program->GetSamplerView(i));
	}

}

void FDx11Renderer::ClearFrameBuffer(FDx11Pass* GBufferPass)
{
	ID3D11RenderTargetView* pRTView[MAX_MULTIPLE_RENDER_TARGETS];
	memset(pRTView, 0, sizeof(pRTView));

	for (unsigned int i = 0; i < MAX_MULTIPLE_RENDER_TARGETS; i++)
	{
		pRTView[i] = GBufferPass->GetRenderTargetView(i);
		if (!pRTView[i])
		{
			break;
		}
	}

	float depth = 1.0;
	UINT8 stencil = 0;
	static float black[4] = { 0.0f, 1.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)

	unsigned int numberOfViews = GBufferPass->GetNumViews();
	for (unsigned int i = 0; i < numberOfViews; ++i)
	{
		mDeviceContext->ClearRenderTargetView(pRTView[i], reinterpret_cast<const float*>(&black));
	}


	UINT ClearFlags = 0;
	ClearFlags |= D3D11_CLEAR_DEPTH;
	ClearFlags |= D3D11_CLEAR_STENCIL;


	mDeviceContext->ClearDepthStencilView(
		GBufferPass->GetDepthStencilView(),
		ClearFlags, depth, static_cast<UINT8>(stencil));

}

void FDx11Renderer::SetRenderStates()
{
	
	mDeviceContext->RSSetState(RenderStates::RSCullClockWise.Get());              // rasterize 
	mDeviceContext->OMSetDepthStencilState(RenderStates::DSSLessEqual.Get(), 0);  // depthStencil 
	mDeviceContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);  // blend
}

int FDx11Renderer::Run()
{
	MSG msg = { 0 };

	mTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mTimer.Tick();

			if (!m_AppPaused)
			{
				//CalculateFrameStats();
				//UpdateScene(m_Timer.DeltaTime());
				Draw();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}




void FDx11Renderer::OnResize()
{
}

bool FDx11Renderer::InitDirect3D(FGraphicContext::Traits* traits, HWND hwnd)
{
	HRESULT hr = S_OK;

	// ����D3D�豸 �� D3D�豸������
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Direct2D��Ҫ֧��BGRA��ʽ
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// ������������
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// ���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = 1;
	//ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		d3dDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mDeviceContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			// Direct3D 11.0 ��API������D3D_FEATURE_LEVEL_11_1������������Ҫ�������Եȼ�11.0�Լ����µİ汾
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mDeviceContext.GetAddressOf());
		}

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	// ����Ƿ�֧�����Եȼ�11.0��11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}
	
	// ��� MSAA֧�ֵ������ȼ�
	mDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m_4xMsaaQuality);	// ע��˴�DXGI_FORMAT_B8G8R8A8_UNORM
	assert(m_4xMsaaQuality > 0);

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(����DXGI1.1)�Ľӿ���
	//ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(����DXGI1.2)���еĽӿ���


	// Ϊ����ȷ���� DXGI������������������Ҫ��ȡ���� D3D�豸 �� DXGI�������������������
	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
	HR(mDevice.As(&dxgiDevice));
	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));


	// ���DXGI_SWAP_CHAIN_DESC��������������
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = traits->width;
	sd.BufferDesc.Height = traits->height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// ע��˴�DXGI_FORMAT_B8G8R8A8_UNORM
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// �Ƿ���4�����ز�����
	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	sd.Flags = 0;
	HR(dxgiFactory1->CreateSwapChain(mDevice.Get(), &sd, mSwapChain.GetAddressOf()));

	OnResize();

	return true;
}





void  FDx11App::Initialize() 
{
	FDx11Renderer::Initialize();
	InitMainCamera();
	InitGameObject();
	InitCommmonConstantBuffer();


}
void FDx11App::InitMainCamera()
{
	mainCamera = new FCamera(Frustum(), "MainCamera");

}

void  FDx11App::InitGameObject()
{
	Ptr<FShape> sphere = new FBox(2.0);
	Ptr<FGeometry> sphereGeom = ShapeGeometryBuilder::Build(sphere.get());
	sphereMesh = new FDx11Mesh(sphereGeom.get());
	sphereMesh->SetLocalPosition(Vec3f(0.0, 1.0, 0.0));
}

void FDx11App::InitCommmonConstantBuffer()
{
	Ptr<UniformDefault> dirLight = new UniformDefault("dirLight", {});
	UniformBasePtr diffuse = new UniformVec4f("diffuse", Vec4f(0.8f, 0.8f, 0.8f, 1.0f));
	UniformBasePtr specular = new UniformVec4f("specular", Vec4f(0.1f, 0.1f, 0.1f, 1.0f));
	UniformBasePtr direction = new UniformVec4f("direction", Vec4f(0.577f, -0.577f, -0.577f, 1.0f));
	UniformBasePtr ambient = new UniformVec4f("direction", Vec4f(0.8f, 0.8f, 0.8f, 1.0f));
	dirLight->AddSubUniform(diffuse);
	dirLight->AddSubUniform(specular);
	dirLight->AddSubUniform(direction);
	dirLight->AddSubUniform(ambient);

	//m_BasicEffect.SetDirLight(dirLight);
	//m_BasicEffect.SetEyePos(m_pCamera->GetPosition());
	//m_BasicEffect.SetProjMatrix(m_pCamera->GetProjXM());
	//m_BasicEffect.SetViewMatrix(m_pCamera->GetViewXM());
	//m_BasicEffect.SetRenderDefault(m_pd3dImmediateContext.Get());
	//m_BasicEffect.InitAll(m_pd3dDevice.Get());
	//struct CBChangesEveryObjectDrawing
	//{
	//	Material material;
	//};

	//struct CBChangesEveryFrame
	//{
	//	DirectX::XMMATRIX view;
	//	DirectX::XMMATRIX world;
	//	DirectX::XMFLOAT4 eyePos;
	//	//float pad;                    //16�ֽڶ���
	//};

	//struct CBChangesOnResize
	//{
	//	DirectX::XMMATRIX proj;
	//};

	//struct CBChangesRarely
	//{
	//	DirectionalLight dirLight;
	//};

}


void FDx11App::Draw()
{
	ExecutePass(singlePass.get());
	sphereMesh->Draw(mDeviceContext.Get());
}
