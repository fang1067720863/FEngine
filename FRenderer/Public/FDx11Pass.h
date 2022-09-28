#pragma once


#include"FDx11Shader.h"
#include"Reference.h"
#include<functional>

#define MAX_MULTIPLE_RENDER_TARGETS 8

class FDx11Pass;
class PassBuilder
{
public:
	struct PassOption {
		std::string name{ "" };
		bool mainTarget{ false };
		unsigned int numViews{ 1 };
		bool enableDepth{ false };
		
	};

	static PassBuilder& Instance()
	{
		static PassBuilder instance;
		return instance;
	}

	PassBuilder(): context(new PassContext())
	{
	
	}
	void SetDevice(FDx11Device* device)
	{
		context->device = device;
	}

	class PassContext:public FReference
	{
	public:

		PassContext()
		{

		}
		using SrvPtr = int32_t;  //resourceSlot
		ComPtr<ID3D11RenderTargetView> mainRenderTargetView;   // 渲染目标视图
		ComPtr<ID3D11DepthStencilView> mainDepthStencilView;   // 深度模板视图
		D3D11_VIEWPORT                 mainScreenViewport;     // 视口
		std::unordered_map<std::string, SrvPtr> srvMap{};
		FDx11Device* device;

		bool AddSrvMap(const std::string& name, SrvPtr srv)
		{
			if (srvMap.find(name) != srvMap.end())
			{
				return false;
			}
			else {
				srvMap[name] = srv;
			}
		}
		SrvPtr GetSrvMap(const std::string& name)
		{
			if (srvMap.find(name) != srvMap.end())
			{
				return srvMap[name];
			}
			else {
				return -1;
			}
		}
	};
	Ptr<FDx11Pass> CreatePass(const PassBuilder::PassOption& option);  // 右值是否好些？


	Ptr<PassContext> context;

};



class FDx11Pass :public FReference
{
	using PassCBUpdateCallback = std::function<void(float dt)>;
public:
	FDx11Pass(Ptr<PassBuilder::PassContext> _context, const PassBuilder::PassOption& _option);
	

	Ptr<PassBuilder::PassContext> globalContext;
	PassBuilder::PassOption option;

	FDx11Pass(const FDx11Device& _device, const D3D11_VIEWPORT& vp);
	FDx11Pass(const std::string& _name, unsigned int numViews, const D3D11_VIEWPORT& vp, const FDx11Device& _device);
	bool InitPass(const std::string& vs, const std::string& ps);
	
	// render target view
	unsigned int GetNumViews() { return mNumViews; }

	int32_t GetRTShaderResourceSlot(const std::string& name)
	{
		return mRTShaderResourceSlots.at(name);
	}


	ID3D11DepthStencilView* GetDepthStencilView() {
		return mDepthStencilView.Get();
	}
	void ResetAll()
	{
		mDepthStencilView.Reset();
		mDepthStencilBuffer.Reset();

	}
	const D3D11_VIEWPORT* GetViewport() const {
		return &mViewport;
	}
	const D3D11_RECT* GetScissorRects() const
	{
		return &mRect;
	}

	FDx11GpuProgram* GetGpuProgram()const
	{
		return mGpuProgram.get();
	}
	bool Begin();
	bool End();
	bool _UseRenderState();
	bool _SetRenderTarget();
	bool _ClearRenderTargetView();
	
	

	void Update(float dt)
	{
		if (updateCB)
		{
			updateCB(dt);
		}
	}
	void SetUpdateCallback(PassCBUpdateCallback cb) { updateCB = cb; }


protected:
	PassCBUpdateCallback updateCB;
	

	bool InitShaderResourceView(ID3D11Device* device);

	bool InitRenderTexture(ID3D11Device* device);



	bool InitGpuProgram(const std::string& vs, const std::string& ps);
	

	// 渲染目标视图
	ID3D11RenderTargetView* mRenderTargetView[MAX_MULTIPLE_RENDER_TARGETS];
	ID3D11Texture2D* mRenderTargetTextures[MAX_MULTIPLE_RENDER_TARGETS];
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	std::unordered_map<std::string, int32_t> mRTShaderResourceSlots;
	D3D11_VIEWPORT mViewport;
	D3D11_RECT mRect;
	unsigned int mNumViews = 0;
	FPtr<FDx11GpuProgram> mGpuProgram;
	std::string mName;

	FDx11Device* mDevice;


	ID3D11ShaderResourceView* gBufferSRV[MAX_MULTIPLE_RENDER_TARGETS];

	ID3D11ShaderResourceView* GetGBufferSRV(int i)
	{
		return gBufferSRV[i];
	}

};