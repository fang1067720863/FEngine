#pragma once

#include"FDx11.h"
class FDx11Rtv
{
public:
	ID3D11RenderTargetView* GetRenderTargetView() {
		return m_pRenderTargetView.Get();
	}

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// ‰÷»æƒø±Í ”Õº

	struct FRtvProperties
	{
		DXGI_FORMAT mRtvFormat;

		// RtvWidth = viewportWidth * mWidthPercentage
		float mWidthPercentage = 1;
		float mHeightPercentage = 1;

		FLOAT mClearColor[4] = { 0,0,0,0 };
	};

	FDx11Rtv(FRtvProperties prop, UINT ClientWidth, UINT ClientHeight, bool ScaledByViewport = true)
	{
		mProperties = prop;

		mClientWidth = ClientWidth;
		mClientHeight = ClientHeight;

		bScaledByViewport = ScaledByViewport;

		if (bScaledByViewport)
		{
			SetViewportAndScissorRect((UINT)(max(ClientWidth * mProperties.mWidthPercentage, 1.0f)), (UINT)(max(ClientHeight * mProperties.mHeightPercentage, 1.0f)));
		}
		else
		{
			SetViewportAndScissorRect(ClientWidth, ClientHeight);
		}
	}

	~FDx11Rtv() {}

	void OnResize(UINT ClientWidth, UINT ClientHeight)
	{
		if (bScaledByViewport)
		{
			mClientWidth = ClientWidth;
			mClientHeight = ClientHeight;
			SetViewportAndScissorRect((UINT)(max(ClientWidth * mProperties.mWidthPercentage, 1.0f)), (UINT)(max(ClientHeight * mProperties.mHeightPercentage, 1.0f)));
		}
	}

	void SetViewportAndScissorRect(UINT Width, UINT Height)
	{
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.Width = (FLOAT)Width;
		mViewport.Height = (FLOAT)Height;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		mScissorRect = { 0, 0, (int)(mViewport.Width), (int)(mViewport.Height) };
	}

	FRtvProperties mProperties;
	
	UINT mClientWidth = 0;
	UINT mClientHeight = 0;

	D3D11_VIEWPORT mViewport;
	D3D11_RECT mScissorRect;

	bool bScaledByViewport = true;

};
class FDx11RtvHeap // render target view
{
public:
	FDx11RtvHeap() = delete;
	FDx11RtvHeap(const FDx11RtvHeap& rhs) = delete;
	FDx11RtvHeap& operator=(const FDx11RtvHeap& rhs) = delete;
	~FDx11RtvHeap() = default;
	std::vector<std::shared_ptr<FDx11Rtv>> mRtvs;
};