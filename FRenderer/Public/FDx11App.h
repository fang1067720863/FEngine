#pragma once

#include"FDx11RenderTarget.h"

#include"FDx11Mesh.h"
#include"FDx11Pass.h"
#include"FTimer.h"

#include"Camera.h"
#include"EventHandler.h"
#include"d3dApp.h"
#include"UtfConverter.h"
#include"Group.h"

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




class GLTFModel;
class FDx11App : public D3DApp
{
public:

	Ptr<FDx11Pass> forwardPass;

	bool InitSinglePass();
	void ExecuteMainPass(FDx11Pass* pass);

	void ClearFrameBuffer(FDx11Pass* GBufferPass);
	void InitCommmonConstantBuffer();
	void InitSamplerResourcePool();
	// ÏñËØ×ÅÉ«Æ÷
	FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	
	~FDx11App()
	{

	}

	


	bool Init() override
	{
		if (!D3DApp::Init())
			return false;
		InitSamplerResourcePool();

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
	Ptr<FGroup> sceneGroup;
	Ptr<FCamera> mainCamera;

	std::shared_ptr<GLTFModel> gltfModel;

	//std::shared_ptr<GLTFModel> gltfModel;

};