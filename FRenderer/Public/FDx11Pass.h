#pragma once


#include"FDx11Shader.h"
#include"Reference.h"
#include<functional>

#define MAX_MULTIPLE_RENDER_TARGETS 8

class FDx11Pass;
class PassBuilder
{
public:
	struct RenderStateSet
	{
		RasterizeStateType rst{ RasterizeStateType::RS_NO_CULL };
		BlendStateType bst{ BlendStateType::ADDITIVE };
		DepthStencilStateType dst{ DepthStencilStateType::LESS_EQUAL };
	};
	struct PassOption {
		std::string name{ "" };
		bool mainTarget{ true };
		unsigned int numViews{ 0 };
		RenderStateSet stateset;
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

		PassContext() = default;
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
				std::cout << "no texture in context" << std::endl;
				return -1;
			}
		}
	};
	Ptr<FDx11Pass> CreatePass(const PassBuilder::PassOption& option);  // 右值是否好些？
	Ptr<PassContext> context;

};



class FDx11Pass :public FReference
{
	friend class PassBuilder;
	
public:

	unsigned int               GetNumViews()   { return mNumViews; }
	PassBuilder::PassContext * GlobalContext() { return globalContext.get(); }
    FDx11GpuProgram*           GetGpuProgram()const{return mGpuProgram.get();}

	bool Begin();
	bool End();
	bool InitPass(const std::string& vs, const std::string& ps);
	void SetConstantBufferSlots(std::vector<std::string>&& slots);
	void AddRTShaderResource(const std::vector<std::string>& targets);
	
protected:
	FDx11Pass(Ptr<PassBuilder::PassContext> _context, const PassBuilder::PassOption& _option);

	bool _InitRenderTexture(ID3D11Device* device);
	bool _InitGpuProgram(const std::string& vs, const std::string& ps);
	bool _AddSRVConstant();

	bool _UseRenderState();
	bool _SetRenderTarget();
	
	ID3D11RenderTargetView*  mRenderTargetView[MAX_MULTIPLE_RENDER_TARGETS];
	ID3D11Texture2D*         mRenderTargetTextures[MAX_MULTIPLE_RENDER_TARGETS];
	FPtr<FDx11GpuProgram>    mGpuProgram;
	D3D11_VIEWPORT           mViewport;
	D3D11_RECT               mRect;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;
	ComPtr<ID3D11Texture2D>        mDepthStencilBuffer;
	
	std::string                    mName;
	unsigned int                   mNumViews{ 0 };
	PassBuilder::PassOption        option;
	std::vector<std::string>       mConstantSlots;

	FDx11Device* mDevice;
	Ptr<PassBuilder::PassContext> globalContext;

};