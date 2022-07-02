#pragma once

#include<unordered_map>
#include<memory>


#include"FDx11RenderTarget.h"

#include"FDx11Mesh.h"
#include"FDx11Pass.h"

#include"FRenderer.h"
#include"FTimer.h"
#include"GraphicContext.h"
#include"ShaderParameter.h"
#include"Camera.h"






class FDx11ShadingState
{
	ComPtr<ID3D11VertexShader> m_pBasicVS;
	ComPtr<ID3D11PixelShader> m_pBasicPS;

	ComPtr<ID3D11InputLayout> m_pPosNormalTexLayout;
	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuse;		                
	ComPtr<ID3D11ShaderResourceView> m_pTextureCube;			            
	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuseDefault;
};

class RenderStates
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	static bool IsInit();

	static void InitAll(ID3D11Device* device);
	// 使用ComPtr无需手工释放

public:
	static ComPtr<ID3D11RasterizerState> RSWireframe;		            // 光栅化器状态：线框模式
	static ComPtr<ID3D11RasterizerState> RSNoCull;			            // 光栅化器状态：无背面裁剪模式
	static ComPtr<ID3D11RasterizerState> RSCullClockWise;	            // 光栅化器状态：顺时针裁剪模式

	static ComPtr<ID3D11SamplerState> SSLinearWrap;			            // 采样器状态：线性过滤
	static ComPtr<ID3D11SamplerState> SSAnistropicWrap;		            // 采样器状态：各项异性过滤

	static ComPtr<ID3D11BlendState> BSNoColorWrite;		                // 混合状态：不写入颜色
	static ComPtr<ID3D11BlendState> BSTransparent;		                // 混合状态：透明混合
	static ComPtr<ID3D11BlendState> BSAlphaToCoverage;	                // 混合状态：Alpha-To-Coverage
	static ComPtr<ID3D11BlendState> BSAdditive;			                // 混合状态：加法混合


	static ComPtr<ID3D11DepthStencilState> DSSLessEqual;		        // 深度/模板状态：允许绘制深度值相等的像素
	static ComPtr<ID3D11DepthStencilState> DSSWriteStencil;		        // 深度/模板状态：写入模板值
	static ComPtr<ID3D11DepthStencilState> DSSDrawWithStencil;	        // 深度/模板状态：对指定模板值的区域进行绘制
	static ComPtr<ID3D11DepthStencilState> DSSNoDoubleBlend;	        // 深度/模板状态：无二次混合区域
	static ComPtr<ID3D11DepthStencilState> DSSNoDepthTest;		        // 深度/模板状态：关闭深度测试
	static ComPtr<ID3D11DepthStencilState> DSSNoDepthWrite;		        // 深度/模板状态：仅深度测试，不写入深度值
	static ComPtr<ID3D11DepthStencilState> DSSNoDepthTestWithStencil;	// 深度/模板状态：关闭深度测试，对指定模板值的区域进行绘制
	static ComPtr<ID3D11DepthStencilState> DSSNoDepthWriteWithStencil;	// 深度/模板状态：仅深度测试，不写入深度值，对指定模板值的区域进行绘制
};

class FDx11Renderer : public FRenderer
{
public:
	 FDx11Renderer(FGraphicContext::Traits * traits,HWND hwnd);

	 void InitSinglePass();
	 void ExecutePass(FDx11Pass* pass);
	 void ClearFrameBuffer(FDx11Pass* pass);
	 void SetRenderStates();

	 void SetGpuProgram(FDx11GpuProgram* program);


	 void Initialize() override;
	 bool InitDirect3D(FGraphicContext::Traits* traits, HWND hwnd);
	 int Run();
	 void OnResize() override;
	
	 void Update() override;
	 void Draw() override;

	 Ptr<FDx11Pass> singlePass;
	 // Direct3D 11
	 ComPtr<ID3D11Device> mDevice;							// D3D11设备
	 ComPtr<ID3D11DeviceContext> mDeviceContext;		// D3D11设备上下文

	 FDx11Device device;

	 ComPtr<IDXGISwapChain> mSwapChain;						// D3D11交换链

	 // 常用资源
	 ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;				// 深度模板缓冲区
	 ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// 渲染目标视图
	 ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;		// 深度模板视图
	 D3D11_VIEWPORT m_ScreenViewport;  

	 float mClientWidth = 800.0;
	 float mClientHeight = 600.0;

	 
	 bool      m_AppPaused;       // 应用是否暂停
	 bool      m_Minimized;       // 应用是否最小化
	 bool      m_Maximized;       // 应用是否最大化
	 bool      m_Resizing;        // 窗口大小是否变化
	 bool	  m_Enable4xMsaa;	 // 是否开启4倍多重采样
	 UINT      m_4xMsaaQuality;   // MSAA支持的质量等级

	 FGraphicContext::Traits* mTraits;
	 HWND mHwnd;

	 FTimer mTimer;         // 计时器
};


class FDx11App : public FDx11Renderer
{
public:
	;
	FDx11App(FGraphicContext::Traits* traits, HWND hwnd) :FDx11Renderer(traits, hwnd) {}
	void InitMainCamera();
	void InitGameObject();
	void InitCommmonConstantBuffer();

	void Initialize() override;
	void Draw()override;

	Ptr<FDx11Mesh> sphereMesh;
	Ptr<FCamera> mainCamera;

};






