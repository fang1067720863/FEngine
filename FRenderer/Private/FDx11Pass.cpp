#include"../Public/FDx11Pass.h"
#include"../Public/FDx11RenderState.h"
#include"../Public/FDx11ResourceFactory.h"

FDx11Pass::FDx11Pass(const FDx11Device& _device,const D3D11_VIEWPORT& vp) :mDevice(_device)
{
	mViewport = vp;
}

FDx11Pass::FDx11Pass(unsigned int numViews, const D3D11_VIEWPORT& vp, const FDx11Device& _device) :mNumViews(numViews), mDevice(_device)
{
	mViewport = vp;
	/*for (uint32_t i = 0; i < mNumViews; i++)
	{
		mRTTextures.push_back(ComPtr<ID3D11Texture2D>());
		mRTV.push_back(ComPtr<ID3D11RenderTargetView>());
	}*/
	//InitPass(mDevice.GetDevice());
}
bool FDx11Pass::InitPass(const std::string& vs, const std::string& ps)
{
	ID3D11Device* device = mDevice.GetDevice();
	if (!InitRenderTexture(device))
	{
		return false;
	}
	

	if (!InitGpuProgram(vs,ps))
	{
		return false;
	}

	
}


bool FDx11Pass::ClearRenderTargetView()
{
	float depth = 1.0;
	UINT8 stencil = 0;
	static float black[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,0,255)

	UINT ClearFlags = 0;
	ClearFlags |= D3D11_CLEAR_DEPTH;
	ClearFlags |= D3D11_CLEAR_STENCIL;
	for (unsigned int i = 0; i < GetNumViews(); ++i)
	{
		mDevice.GetDeviceContext()->ClearRenderTargetView(mRenderTargetView[i], reinterpret_cast<const float*>(&black));
	}

	mDevice.GetDeviceContext()->ClearDepthStencilView(
		mDepthStencilView.Get(),
		ClearFlags, depth, static_cast<UINT8>(stencil));
	
	return true;
}

bool FDx11Pass::InitShaderResourceView(ID3D11Device* device)
{
	return true;
}
bool FDx11Pass::InitRenderTexture(ID3D11Device* device)
{
	if (mNumViews == 1)
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

	D3D11_SUBRESOURCE_DATA sd;
	uint32_t* pData = nullptr;
	sd.pSysMem = pData;
	sd.SysMemPitch = 128 * sizeof(uint32_t);
	sd.SysMemSlicePitch = 128 * 128 * sizeof(uint32_t);

	for (unsigned int i = 0; i < mNumViews; i++)
	{
		HR(device->CreateTexture2D(&texArrayDesc, nullptr, &mRenderTargetTextures[i]));
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mViewport.Width;
	depthStencilDesc.Height = mViewport.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// 创建深度缓冲区以及深度模板视图
	HR(device->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));

	for (UINT i = 0; i < mNumViews; i++)
	{
		HR(device->CreateRenderTargetView(mRenderTargetTextures[i], nullptr, &mRenderTargetView[i]));
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	HR(device->CreateDepthStencilView(mDepthStencilBuffer.Get(), &depthStencilViewDesc, mDepthStencilView.GetAddressOf()));


	// init shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDepthDesc;
	srvDepthDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDepthDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDepthDesc.Texture2D.MostDetailedMip = 0;
	srvDepthDesc.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvTextureDesc;
	srvTextureDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvTextureDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvTextureDesc.Texture2D.MostDetailedMip = 0;
	srvTextureDesc.Texture2D.MipLevels = 1;
	for (UINT i = 0; i < mNumViews; i++)
	{

		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		HR(device->CreateShaderResourceView(mRenderTargetTextures[i], &srvTextureDesc, &shaderResourceView));
		int32_t slot = ShaderResoucePool::Instance().CreateShaderResouce(shaderResourceView);
		std::string name = "gBuffer" + std::to_string(slot);
		mRTShaderResourceSlots[name] = slot;
	}

	ComPtr<ID3D11ShaderResourceView> srvDepth;
	HR(device->CreateShaderResourceView(mDepthStencilBuffer.Get(), &srvDepthDesc, &srvDepth));
	int32_t slot = ShaderResoucePool::Instance().CreateShaderResouce(srvDepth);
	std::string name = "gBuffer" + std::string("Depth");
	mRTShaderResourceSlots[name] = slot;
	return true;
}


bool FDx11Pass::UseRenderState()
{
	auto dsState = DepthStencilStateResoucePool::Instance().GetResource(DepthStencilStateType::LESS_EQUAL);
	auto bsState = BlendStateResoucePool::Instance().GetResource(BlendStateType::ADDITIVE);
	auto rsState = RasterStateResoucePool::Instance().GetResource(RasterizeStateType::RS_NO_CULL);
	mDevice.GetDeviceContext()->RSSetState(rsState.Get());
	mDevice.GetDeviceContext()->OMSetDepthStencilState(dsState.Get(), 0);
	mDevice.GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	return true;
}

bool FDx11Pass::InitGpuProgram(const std::string& vs, const std::string& ps)
{
	mGpuProgram = new FDx11GpuProgram(mDevice,vs,ps);
	return true;

}