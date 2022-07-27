#pragma once
//#include"FDx11Renderer.h"
#include"FDx11RenderTarget.h"

#include"FDx11Mesh.h"
#include"FDx11Pass.h"
#include"FTimer.h"

#include"Camera.h"
#include"EventHandler.h"
#include"d3dApp.h"
#include"UtfConverter.h"

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

	Ptr<FDx11Pass> forwardPass;

	bool InitSinglePass();
	void ExecuteMainPass(FDx11Pass* pass);

	void ClearFrameBuffer(FDx11Pass* GBufferPass);
	void InitCommmonConstantBuffer();
	// ÏñËØ×ÅÉ«Æ÷
	FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	
	~FDx11App()
	{

	}

	bool Init() override
	{
		if (!D3DApp::Init())
			return false;

		FindGlobalPath();
		if (!InitSinglePass())
			return false;
		if (!InitGameObject())
			return false;
		
		InitCommmonConstantBuffer();
		
		return true;
	}
	void OnResize()
	{
		D3DApp::OnResize();
	}
	void UpdateScene(float dt) override;
	
	bool InitGameObject();

	void DrawScene() override;
	Ptr<EventHandler> flyController;

private:
	Ptr<FDx11Mesh> boxMesh;
	Ptr<FDx11Mesh> triangleMesh;
	Ptr<FCamera> mainCamera;

};