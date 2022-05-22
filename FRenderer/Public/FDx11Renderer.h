#pragma once

#include<unordered_map>
#include<memory>

#include"GraphicContext.h"
#include"FDx11RenderTarget.h"
#include"FDx11Shader.h"
#include"FRenderer.h"

#include"FDx11Mesh.h"
#include"FDx11Pass.h"






class FDx11ShadingState
{
	ComPtr<ID3D11VertexShader> m_pBasicVS;
	ComPtr<ID3D11PixelShader> m_pBasicPS;

	ComPtr<ID3D11InputLayout> m_pPosNormalTexLayout;
	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuse;		                
	ComPtr<ID3D11ShaderResourceView> m_pTextureCube;			            
	ComPtr<ID3D11ShaderResourceView> m_pTextureDiffuseDefault;
};

class FDx11Renderer : public FRenderer
{
public:
	 FDx11Renderer(FGraphicContext::Traits * traits,HWND hwnd);

	 void InitGBufferPass(FDx11Pass* gBufferPass);
	 void ExecuteGBufferPass(FDx11Pass* GBufferPass);
	 void ClearFrameBuffer(FDx11Pass* GBufferPass);
	 void SetRenderStates(const RenderStateSet* renderStates);

	 void SetGpuProgram(FDx11GpuProgram* program);

	 void Initialize() override;
	 void InitGameObject()
	 {
		 Ptr<FShape> sphere = new FBox(2.0);
		 Ptr<FGeometry> sphereGeom = ShapeGeometryBuilder::Build(sphere.get());
		 sphereMesh = new FDx11Mesh(sphereGeom.get());
		 sphereMesh->SetLocalPosition(Vec3f(0.0, 1.0, 0.0));
	 }

	 void OnResize() override;
	 bool InitDirect3D(FGraphicContext::Traits* traits, HWND hwnd);
	 void Update(const FTimer* gt) override;
	 void Draw(const FTimer* gt) override;

	 Ptr<FDx11Mesh> sphereMesh;
	 // Direct3D 11
	 ComPtr<ID3D11Device> mDevice;							// D3D11�豸
	 ComPtr<ID3D11DeviceContext> mDeviceContext;		// D3D11�豸������
	 ComPtr<IDXGISwapChain> mSwapChain;						// D3D11������

	 // ������Դ
	 ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;				// ���ģ�建����
	 ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;		// ��ȾĿ����ͼ
	 ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;		// ���ģ����ͼ
	 D3D11_VIEWPORT m_ScreenViewport;  
	 
	 bool      m_AppPaused;       // Ӧ���Ƿ���ͣ
	 bool      m_Minimized;       // Ӧ���Ƿ���С��
	 bool      m_Maximized;       // Ӧ���Ƿ����
	 bool      m_Resizing;        // ���ڴ�С�Ƿ�仯
	 bool	  m_Enable4xMsaa;	 // �Ƿ���4�����ز���
	 UINT      m_4xMsaaQuality;   // MSAA֧�ֵ������ȼ�

};







