#pragma once


#include"FDx11Shader.h"
#include"Reference.h"

#define MAX_MULTIPLE_RENDER_TARGETS 8


class FDx11Pass:public FReference
{
public:

	FDx11Pass(const FDx11Device& _device, const D3D11_VIEWPORT& vp);
	FDx11Pass(unsigned int numViews,  const D3D11_VIEWPORT& vp,const FDx11Device& _device);
	bool InitPass(const std::string& vs, const std::string& ps);
	// render target view
	unsigned int GetNumViews() { return mNumViews; }
	ID3D11RenderTargetView** GetRenderTargetViewAddress() {
		return mRenderTargetView; }
	ID3D11RenderTargetView* GetRenderTargetView(unsigned int i) {
		return mRenderTargetView[i];
	}
	ID3D11Texture2D* GetDepthStencilBuffer() {
		return mDepthStencilBuffer.Get();
	}

	ID3D11DepthStencilView* GetDepthStencilView() {
		return mDepthStencilView.Get();
	}
	void ResetAll()
	{
		mDepthStencilView.Reset();
		mDepthStencilBuffer.Reset();
		mMainRTV.Reset();

	}
	const D3D11_VIEWPORT* GetViewport() const{
		return &mViewport;
	}
	const D3D11_RECT* GetScissorRects() const
	{
		return &mRect;
	}
	FDx11GpuProgram* GetGpuProgram() const
	{
		return mGpuProgram.get();
	}
	FDx11VertexInputLayout* GetInputLayout() const
	{
		return mVInputLayout.get();
	}
	ComPtr<ID3D11RenderTargetView> mMainRTV;
	ComPtr<ID3D11Texture2D> mMainRTTextures;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	
protected:

	bool InitRenderTargetView(ID3D11Device* device);

	bool InitRenderTexture(ID3D11Device* device);

	bool InitRenderState();

	bool InitGpuProgram(const std::string& vs, const std::string& ps);

	bool InitVertexInputLayout();

	// 渲染目标视图
	ID3D11RenderTargetView* mRenderTargetView[MAX_MULTIPLE_RENDER_TARGETS];		
	ID3D11Texture2D* mRenderTargetTextures[MAX_MULTIPLE_RENDER_TARGETS];

	
	// 深度模板视图
	

	D3D11_VIEWPORT mViewport;
	D3D11_RECT mRect;	

	unsigned int mNumViews = 0;

	FPtr<FDx11GpuProgram> mGpuProgram;
	FPtr<FDx11VertexInputLayout> mVInputLayout;

	const FDx11Device& mDevice;

	std::string mProgram;
	//RenderStateSet* mRenderStates;
};