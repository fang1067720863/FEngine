#include"FDx11App.h"
#include<D3DX11tex.h>
#include"Shape.h"
#include"GltfReader.h"

void FDx11App::ExecuteMainPass(FDx11Pass* pass)
{

	// Set Viewport
	//device.GetDeviceContext()->RSSetViewports(1, &m_ScreenViewport);
	//device.GetDeviceContext()->RSSetScissorRects(1, pass->GetScissorRects());

	// Clear&Set FrameBuffer

	//device.GetDeviceContext()->OMSetRenderTargets(1, pass->mMainRTV.GetAddressOf(), pass->GetDepthStencilView());
	//m_pDevice.GetDeviceContext()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	// Render statemain
	//SetRenderStates(gBufferPass->GetRenderStates());
	//SetRenderStates();
	//m_pDevice.GetDeviceContext()->IASetInputLayout(pass->GetGpuProgram()->inputLayout.Get());
	
	// inputLayout
	m_pDevice.GetDeviceContext()->IASetInputLayout(pass->GetGpuProgram()->inputLayout.Get());
	// shader & shader resource
	pass->GetGpuProgram()->UseProgram();
	pass->UseRenderState();
	

}




void FDx11App::ClearFrameBuffer(FDx11Pass* pass)
{


	float depth = 1.0;
	UINT8 stencil = 0;
	static float black[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,0,255)

	unsigned int numberOfViews = pass->GetNumViews();
	/*for (unsigned int i = 0; i < numberOfViews; ++i)
	{
		device.GetDeviceContext()->ClearRenderTargetView(GBufferPass->GetRenderTargetView(i), reinterpret_cast<const float*>(&black));
	}*/
	m_pDevice.GetDeviceContext()->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&black));

	UINT ClearFlags = 0;
	ClearFlags |= D3D11_CLEAR_DEPTH;
	ClearFlags |= D3D11_CLEAR_STENCIL;


	m_pDevice.GetDeviceContext()->ClearDepthStencilView(
		pass->GetDepthStencilView(),
		ClearFlags, depth, static_cast<UINT8>(stencil));
	m_pDevice.GetDeviceContext()->ClearDepthStencilView(
		m_pDepthStencilView.Get(),
		ClearFlags, depth, static_cast<UINT8>(stencil));
}

FDx11App::FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

bool FDx11App::InitSinglePass()
{
	forwardPass = new FDx11Pass(1, m_ScreenViewport,m_pDevice);
	forwardPass->InitPass("DefaultVertex", "PbrPS");
	_InitForwardPassShaderInput();

	skyPass = new FDx11Pass(1, m_ScreenViewport, m_pDevice);
	skyPass->InitPass("skybox_vs", "skybox_ps");
	_InitSkyPassShaderInput();
	return true;
}
void FDx11App::_InitSkyPassShaderInput()
{

	Ptr<ConstantBufferObject> skyMVP = ConstantBufferPool::Instance().CreateDeviceResource("skyMVP", sizeof(Mat4), m_pDevice);

	skyMVP->Upload<Mat4>(
		Mat4{
			mainCamera->GetViewMatrix() * mainCamera->GetProjMatrix()
		}
	);
	skyPass->GetGpuProgram()->AddConstantBuffer(skyMVP);

}
void FDx11App::_InitForwardPassShaderInput()
{
	

	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::Instance().CreateDeviceResource("frame", sizeof(CBChangesEveryFrame), m_pDevice);

	frameCBO->Upload<CBChangesEveryFrame>(
		CBChangesEveryFrame{
		mainCamera->GetViewMatrix(), Vec4f(mainCamera->GetEyePos(), 1.0f)
		}
	);
	Ptr<ConstantBufferObject> resizeCBO = ConstantBufferPool::Instance().CreateDeviceResource("onResize", sizeof(CBChangesOnResize), m_pDevice);
	resizeCBO->Upload<CBChangesOnResize>(CBChangesOnResize{ mainCamera->GetProjMatrix() });

	// todo 修改rpi主动修改rhi
	Ptr<ConstantBufferObject> lightCBO = ConstantBufferPool::Instance().CreateDeviceResource("light", sizeof(Light), m_pDevice);
	lightCBO->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });

	Ptr<ConstantBufferObject> worldCBO = ConstantBufferPool::Instance().CreateDeviceResource("world", sizeof(CBEveryObject), m_pDevice);
	worldCBO->Upload<CBEveryObject>(CBEveryObject{ Mat4() });


	forwardPass->GetGpuProgram()->AddConstantBuffer(frameCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(resizeCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(lightCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(worldCBO);

}


bool FDx11App::InitGameObject()
{

	sceneGroup = new FGroup("SceneData");

	mainCamera = new FCamera(Frustum(), "MainCamera");
	flyController = new FlyCameraController(mainCamera);
	FlyCameraController* controller = dynamic_cast<FlyCameraController*>(flyController.get());
	if (controller)
	{
		controller->SetHomePosition(Vec3f(4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0));
	}

	FBox box(Vec3f(1.0f));
	FGeometry* skyboxGeom = ShapeGeometryBuilder::instance().BuildBox(box);
	skyboxGeom->SetMaterialType(MaterialType::SkyBox);
	skyboxGeom->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model"));

	skybox = new FDx11Mesh(skyboxGeom, m_pDevice);

	

	GltfReader reader;
	gltfModel = make_shared<GLTFModel>();
	reader.loadAssets("D:\\GitProject\\FEngine\\Assets\\PbrBox\\BoomBox.gltf", *gltfModel.get());
	Ptr<FNode> gltfMesh = new FDx11Mesh(dynamic_cast<FGeometry*>((gltfModel->node).get()), m_pDevice);
	gltfMesh->SetScale(Vec3f(20.0f));
	sceneGroup->AddChild(gltfMesh);
	gltfMesh->SetUpdateCallback(
		[gltfMesh](float dt) {
				static float theta = 0.0f;
				theta += 0.005f * dt;
				gltfMesh->SetRotate(theta, 0.0f, 1.0f, 0.0f);
		});
	
	return true;
}

void FDx11App::DrawScene()
{
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_pDevice.deviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pDevice.deviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ExecuteMainPass(forwardPass.get());
	sceneGroup->Draw();
	ExecuteMainPass(skyPass.get());
	skybox->Draw();

	HR(m_pSwapChain->Present(0, 0));
}



void FDx11App::UpdateScene(float dt)
{
	// handle events
	
	while (!eventQueue.Empty())
	{
		Ptr<Event> evt = eventQueue.TakeoutEvent();
		evt->Handled(*flyController);
	}
	sceneGroup->Update(dt);


	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::Instance().GetResource("frame");
	frameCBO->Upload<CBChangesEveryFrame>(
		CBChangesEveryFrame{
		mainCamera->GetViewMatrix(),
		Vec4f(mainCamera->GetEyePos(),1.0f)
		}
	);
	Ptr<ConstantBufferObject> skyMVP = ConstantBufferPool::Instance().GetResource("skyMVP"); 
	Mat4 V = mainCamera->GetViewMatrix();
	Mat4 VP{
		rotate(V.GetRotate()) * mainCamera->GetProjMatrix()
	};
	VP.transpose();
	//V.value[3] = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	skyMVP->Upload<Mat4>(
		VP
	);
}
