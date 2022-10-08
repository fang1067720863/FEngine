#include"../Public/FDx11Pass.h"
#include"../Public/FDx11RenderState.h"
#include"../Public/FDx11ResourceFactory.h"



Ptr<FDx11Pass> PassBuilder::CreatePass(const PassBuilder::PassOption& option)
{
	return Ptr<FDx11Pass>(new FDx11Pass(context, option));
}

FDx11Pass::FDx11Pass(Ptr<PassBuilder::PassContext> _context, const PassBuilder::PassOption& _option)
{
	globalContext = _context;
	option = _option;
	mDevice = _context->device;
	mNumViews = _option.numViews;
}

bool FDx11Pass::End()
{
	mDevice->deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	return true;
}

bool FDx11Pass::Begin()
{
	_SetRenderTarget();
	_UseRenderState();
	mDevice->GetDeviceContext()->IASetInputLayout(mGpuProgram->inputLayout.Get());
	mGpuProgram->UseProgram();
	return true;
}
bool FDx11Pass::InitPass(const std::string& vs, const std::string& ps)
{
	ID3D11Device* device = mDevice->GetDevice();
	if (!_InitRenderTexture(device))
	{
		return false;
	}
	if (!_InitGpuProgram(vs,ps))
	{
		return false;
	}
}

bool FDx11Pass::_InitGpuProgram(const std::string& vs, const std::string& ps)
{
	mGpuProgram = new FDx11GpuProgram(*mDevice, vs, ps);
	return true;

}

bool FDx11Pass::_SetRenderTarget()
{
	float depth = 1.0;
	UINT8 stencil = 0;
	static float black[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,0,255)

	UINT ClearFlags = 0;
	ClearFlags |= D3D11_CLEAR_DEPTH;
	ClearFlags |= D3D11_CLEAR_STENCIL;

	if (option.mainTarget)
	{
		mDevice->deviceContext->ClearRenderTargetView(globalContext->mainRenderTargetView.Get(), black);
		mDevice->deviceContext->ClearDepthStencilView(globalContext->mainDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mDevice->deviceContext->OMSetRenderTargets(1, globalContext->mainRenderTargetView.GetAddressOf(), globalContext->mainDepthStencilView.Get());
		mDevice->deviceContext->RSSetViewports(1, &globalContext->mainScreenViewport);
	
	}
	else {

		for (unsigned int i = 0; i < GetNumViews(); ++i)
		{
			mDevice->GetDeviceContext()->ClearRenderTargetView(mRenderTargetView[i], reinterpret_cast<const float*>(&black));
		}

		mDevice->GetDeviceContext()->ClearDepthStencilView(
			mDepthStencilView.Get(),
			ClearFlags, depth, static_cast<UINT8>(stencil));

		mDevice->deviceContext->OMSetRenderTargets(mNumViews, mRenderTargetView, mDepthStencilView.Get());
		// 设置视口变换
		D3D11_VIEWPORT m_ScreenViewport;
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = static_cast<float>(800);
		m_ScreenViewport.Height = static_cast<float>(600);
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		mDevice->deviceContext->RSSetViewports(1, &m_ScreenViewport);
		
	}

	return true;

}


bool FDx11Pass::_InitRenderTexture(ID3D11Device* device)
{
	if (mNumViews == 0)
	{
		return true;
	}

	// 后面改成TextureArray
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = 800;
	texArrayDesc.Height = 600;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 1;
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;      // 不使用多重采样
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0; // 指定需要生成mipmap

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		// Setup the description of the render target view.
	renderTargetViewDesc.Format = texArrayDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (unsigned int i = 0; i < mNumViews; i++)
	{
		HR(device->CreateTexture2D(&texArrayDesc, nullptr, &mRenderTargetTextures[i]));
	}

	for (UINT i = 0; i < mNumViews; i++)
	{
		HR(device->CreateRenderTargetView(mRenderTargetTextures[i], nullptr, &mRenderTargetView[i]));
		D3D11SetDebugObjectName(mRenderTargetView[i], "mRenderTargetView"+ std::to_string(i));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = texArrayDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	for (UINT i = 0; i < mNumViews; i++)
	{

		ComPtr<ID3D11ShaderResourceView> shaderResourceView{ nullptr };
		HR(device->CreateShaderResourceView(mRenderTargetTextures[i], &shaderResourceViewDesc, shaderResourceView.GetAddressOf()));
		int32_t slot = ShaderResoucePool::Instance().CreateShaderResouce(shaderResourceView);
		globalContext->AddSrvMap("gBuffer" + std::to_string(i), slot);
		
		std::cout << slot << endl;
	}


	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = 800;
	depthStencilDesc.Height = 600;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	// 创建深度缓冲区以及深度模板视图
	HR(device->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	HR(device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDepthDesc;
	srvDepthDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDepthDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDepthDesc.Texture2D.MostDetailedMip = 0;
	srvDepthDesc.Texture2D.MipLevels = depthStencilDesc.MipLevels;
	
	ComPtr<ID3D11ShaderResourceView> srvDepth;
	HR(device->CreateShaderResourceView(mDepthStencilBuffer.Get(), &srvDepthDesc, &srvDepth));
	
	int32_t slot = ShaderResoucePool::Instance().CreateShaderResouce(srvDepth);
	globalContext->AddSrvMap("gBufferDepth", slot);
	std::cout << slot << endl;

	return true;
}


bool FDx11Pass::_UseRenderState()
{
	auto dsState = DepthStencilStateResoucePool::Instance().GetResource(option.stateset.dst);
	auto bsState = BlendStateResoucePool::Instance().GetResource(option.stateset.bst);
	auto rsState = RasterStateResoucePool::Instance().GetResource(option.stateset.rst);	

	mDevice->GetDeviceContext()->RSSetState(rsState.Get());
	mDevice->GetDeviceContext()->OMSetDepthStencilState(dsState.Get(), 0);
	mDevice->GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	return true;
}



