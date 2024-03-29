#pragma once

#include"FDx11RenderTarget.h"
#include"FDx11Mesh.h"
#include"FTimer.h"

#include"Camera.h"
#include"EventHandler.h"
#include"d3dApp.h"
#include"UtfConverter.h"
#include"Group.h"
#include"Constant.h"


class GLTFModel;

class FDx11App : public D3DApp
{
public:
	FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	
	~FDx11App(){}

	bool Init() override;
	
	void OnResize(){D3DApp::OnResize();}
	void UpdateScene(float dt) override;
	
	bool InitGameObject();

	void DrawScene() override;
	//Ptr<EventHandler> flyController;

	vector<Ptr<EventHandler>> controllers;

private:
	Ptr<FDx11Mesh> boxMesh;
	Ptr<FGroup> sceneGroup;
	Ptr<FCamera> mainCamera;
	Ptr<FNode> skybox;
	Ptr<FNode> renderQuad;

	std::shared_ptr<GLTFModel> gltfModel;
};