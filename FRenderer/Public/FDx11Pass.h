#pragma once


#include"FDx11Shader.h"

#include"Reference.h"

#define MAX_MULTIPLE_RENDER_TARGETS 8


class FDx11Pass:public FReference
{
public:

	FDx11Pass(const FDx11Device& _device, const D3D11_VIEWPORT& vp);
	FDx11Pass(unsigned int numViews,  const D3D11_VIEWPORT& vp,const FDx11Device& _device);
	bool InitPass(ID3D11Device* device);
	// render target view
	unsigned int GetNumViews() { return mNumViews; }
	ID3D11RenderTargetView** GetRenderTargetViewAddress() { 
		return mRenderTargetView; }
	ID3D11RenderTargetView* GetRenderTargetView(unsigned int i) {
		return mRenderTargetView[i];
	}
	ID3D11DepthStencilView* GetDepthStencilBuffer() {
		return mDepthStencilView.Get();
	}

	ID3D11DepthStencilView* GetDepthStencilView() {
		return mDepthStencilView.Get();
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

	
protected:

	bool InitRenderTargetView(ID3D11Device* device);

	bool InitRenderTexture(ID3D11Device* device);

	bool InitRenderState();

	bool InitGpuProgram();

	bool InitVertexInputLayout();

	// ��ȾĿ����ͼ
	ID3D11RenderTargetView* mRenderTargetView[MAX_MULTIPLE_RENDER_TARGETS];		
	ID3D11Texture2D* mRenderTargetTextures[MAX_MULTIPLE_RENDER_TARGETS];
	// ���ģ����ͼ
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;		
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;		

	D3D11_VIEWPORT mViewport;
	D3D11_RECT mRect;	

	unsigned int mNumViews = 0;

	FPtr<FDx11GpuProgram> mGpuProgram;
	FPtr<FDx11VertexInputLayout> mVInputLayout;

	const FDx11Device& mDevice;
	//RenderStateSet* mRenderStates;
};