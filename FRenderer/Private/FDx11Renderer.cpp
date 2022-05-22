
#include"FDx11Renderer.h"
#include"FDx11BufferObject.h"
 



FDx11Renderer::FDx11Renderer(FGraphicContext::Traits* traits, HWND hwnd)
{
	InitDirect3D(traits, hwnd);
}

void FDx11Renderer::Update(const FTimer* gt)
{

}

void FDx11Renderer::Draw(const FTimer* gt)
{

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

void FDx11Renderer::SetRenderStates(const RenderStateSet* renderStates)
{
	mDeviceContext->RSSetState(renderStates->mRasterizeState.Get());   // rasterize 
	mDeviceContext->OMSetDepthStencilState(renderStates->mDepthStencilState.Get(), 0);  // depthStencil 
	mDeviceContext->OMSetBlendState(renderStates->mBlendState.Get(), nullptr, 0xFFFFFFFF);  // blend
}

void FDx11Renderer::ExecuteGBufferPass(FDx11Pass* gBufferPass)
{
	// Set Viewport
	mDeviceContext->RSSetViewports(gBufferPass->GetNumViews(), gBufferPass->GetViewport());
	mDeviceContext->RSSetScissorRects(1,gBufferPass->GetScissorRects());
	
	// Clear&Set FrameBuffer
	ClearFrameBuffer(gBufferPass);
	mDeviceContext->OMSetRenderTargets(gBufferPass->GetNumViews(), gBufferPass->GetRenderTargetViewAddress(), gBufferPass->GetDepthStencilView());

	// Render state
	SetRenderStates(gBufferPass->GetRenderStates());
	// shader & shader resource
	SetGpuProgram(gBufferPass->GetGpuProgram());

	// Draw GameObject

}

void FDx11Renderer::InitGBufferPass(FDx11Pass* gBufferPass)
{
	gBufferPass = new FDx11Pass(3);
	// init gbuffer constant

}
void FDx11Renderer::SetGpuProgram(FDx11GpuProgram* program)
{
	// shader
	mDeviceContext->VSSetShader(program->GetVertexShader(), program->GetClassInstance(ShaderType::Vertex),
		program->GetNumInstance(ShaderType::Vertex));
	mDeviceContext->PSSetShader(program->GetPixelShader(), program->GetClassInstance(ShaderType::Pixel),
		program->GetNumInstance(ShaderType::Pixel));

	// 一个slot 只设置一块resource element ,不设置成Array magicNumber = 1
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

void FDx11Renderer::Initialize()
{


		//DirectionalLight dirLight;
		//dirLight.ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		//dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		//dirLight.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		//dirLight.direction = XMFLOAT3(0.577f, -0.577f, -0.577f);

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
		//	//float pad;                    //16字节对齐
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


void FDx11Renderer::OnResize()
{
}

bool FDx11Renderer::InitDirect3D(FGraphicContext::Traits* traits, HWND hwnd)
{
	HRESULT hr = S_OK;

	// 创建D3D设备 和 D3D设备上下文
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Direct2D需要支持BGRA格式
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// 驱动类型数组
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// 特性等级数组
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
			// Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
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

	// 检测是否支持特性等级11.0或11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}
	
	// 检测 MSAA支持的质量等级
	mDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m_4xMsaaQuality);	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
	assert(m_4xMsaaQuality > 0);

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(包含DXGI1.1)的接口类
	//ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(包含DXGI1.2)特有的接口类


	// 为了正确创建 DXGI交换链，首先我们需要获取创建 D3D设备 的 DXGI工厂，否则会引发报错：
	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
	HR(mDevice.As(&dxgiDevice));
	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));


	// 填充DXGI_SWAP_CHAIN_DESC用以描述交换链
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = traits->width;
	sd.BufferDesc.Height = traits->height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// 是否开启4倍多重采样？
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


