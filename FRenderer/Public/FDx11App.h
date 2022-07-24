#pragma once
//#include"FDx11Renderer.h"
#include"FDx11RenderTarget.h"

#include"FDx11Mesh.h"
#include"FDx11Pass.h"
#include"FTimer.h"

#include"Camera.h"
#include"EventHandler.h"
#include"d3dApp.h"

struct CBChangesEveryFrame
{
	Mat4 view;
	Mat4 world;
	Vec4f eyePos;
};

struct CBChangesOnResize
{
	Mat4 proj;
};

struct Light
{
	Vec4f ambient;
	Vec4f diffuse;
	Vec4f specular;
	Vec4f direction;
};
struct Material
{
	Vec4f Ambient;
	Vec4f Diffuse;
	Vec4f Specular;
	Vec4f Reflect;
};

class FDx11App : public D3DApp
{
public:

	void InitMainCamera();
	bool InitSinglePass();

	Ptr<FDx11Mesh> boxMesh;
	Ptr<FDx11Mesh> triangleMesh;
	Ptr<FCamera> mainCamera;

	Ptr<FDx11Pass> forwardPass;

	ComPtr<ID3D11InputLayout> m_pVertexLayout;	// 顶点输入布局
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 顶点缓冲区
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// 顶点着色器
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	void ExecuteMainPass(FDx11Pass* pass);
	//void ExecutePass(FDx11Pass* gBufferPass);
	void SetGpuProgram(FDx11GpuProgram* program);
	void ClearFrameBuffer(FDx11Pass* GBufferPass);
	// 像素着色器
	FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	
	~FDx11App()
	{

	}

	bool Init()
	{

		if (!D3DApp::Init())
			return false;

		if (!InitSinglePass())
			return false;
		/*if (!InitEffect())
			return false;*/
		if (!InitResource())
			return false;
		InitCommmonConstantBuffer();
		

		return true;
	}
	void OnResize()
	{
		D3DApp::OnResize();
	}
	void UpdateScene(float dt) override;
	
	

	bool InitResource();
	void InitCommmonConstantBuffer();
	void DrawScene() override;
	Ptr<EventHandler> flyController;

};