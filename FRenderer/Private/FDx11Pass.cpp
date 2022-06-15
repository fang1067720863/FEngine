#include"../Public/FDx11Pass.h"

FDx11Pass::FDx11Pass()
{

}

FDx11Pass::FDx11Pass(unsigned int numViews) :mNumViews(numViews)
{

}
bool FDx11Pass::InitPass(ID3D11Device* device)
{
	if (InitRenderTexture(device))
	{
		return false;
	}
	if (InitRenderTargetView(device))
	{
		return false;
	}
	if (InitRenderState())
	{
		return false;
	}
	if (InitGpuProgram())
	{
		return false;
	}

	
}

bool FDx11Pass::InitRenderTargetView(ID3D11Device *device)
{
	if (mNumViews == 0)
	{
		return true;
	}
	for (UINT i = 0; i < mNumViews; i++)
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		HR(device->CreateRenderTargetView(mRenderTargetTextures[i], nullptr, &mRenderTargetView[i]));
	}
	HR(device->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf()));
	return true;
}

bool FDx11Pass::InitRenderTexture(ID3D11Device* device)
{
	if (mNumViews == 0)
	{
		return true;
	}
	
	// 后面改成TextureArray
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = 128;
	texArrayDesc.Height = 128;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 1;
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;      // 不使用多重采样
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0; // 指定需要生成mipmap

	D3D11_SUBRESOURCE_DATA sd;
	uint32_t* pData = nullptr;
	sd.pSysMem = pData;
	sd.SysMemPitch = 128 * sizeof(uint32_t);
	sd.SysMemSlicePitch = 128 * 128 * sizeof(uint32_t);

	for (unsigned int i = 0; i < mNumViews; i++)
	{
		device->CreateTexture2D(&texArrayDesc, nullptr, &mRenderTargetTextures[i]);
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mViewport.Width;
	depthStencilDesc.Height = mViewport.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	//if (m_Enable4xMsaa)
	//	depthStencilDesc.SampleDesc.Count = 4;
	//	depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// 创建深度缓冲区以及深度模板视图
	HR(device->CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf()));

	return true;
}


bool FDx11Pass::InitRenderState()
{
	return true;
}

bool FDx11Pass::InitGpuProgram()
{
	return true;

}