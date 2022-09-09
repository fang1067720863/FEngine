#include"FDx11App.h"
#include<D3DX11tex.h>
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
	

}




void FDx11App::ClearFrameBuffer(FDx11Pass* GBufferPass)
{


	float depth = 1.0;
	UINT8 stencil = 0;
	static float black[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,0,255)

	unsigned int numberOfViews = GBufferPass->GetNumViews();
	/*for (unsigned int i = 0; i < numberOfViews; ++i)
	{
		device.GetDeviceContext()->ClearRenderTargetView(GBufferPass->GetRenderTargetView(i), reinterpret_cast<const float*>(&black));
	}*/
	m_pDevice.GetDeviceContext()->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&black));

	UINT ClearFlags = 0;
	ClearFlags |= D3D11_CLEAR_DEPTH;
	ClearFlags |= D3D11_CLEAR_STENCIL;


	m_pDevice.GetDeviceContext()->ClearDepthStencilView(
		GBufferPass->GetDepthStencilView(),
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
	return true;
}
void FDx11App::InitSamplerResourcePool()
{
	ComPtr<ID3D11SamplerState> samplerLinearWrap;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	// 线性过滤模式
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pDevice.GetDevice()->CreateSamplerState(&sampDesc, samplerLinearWrap.GetAddressOf()));

	SamplerResoucePool::Instance().CreateResouce(SamplerType::SSLinearWrap, samplerLinearWrap);

	ComPtr<ID3D11SamplerState> samplerAnistropicWrap;

	// 各向异性过滤模式
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pDevice.GetDevice()->CreateSamplerState(&sampDesc, samplerAnistropicWrap.GetAddressOf()));
	SamplerResoucePool::Instance().CreateResouce(SamplerType::SSAnistropicWrap, samplerAnistropicWrap);

}
void FDx11App::InitCommmonConstantBuffer()
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
	mainCamera = new FCamera(Frustum(), "MainCamera");
	//mainCamera->lookAt<float>(Vec3f(-4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
	
	flyController = new FlyCameraController(mainCamera);
	FlyCameraController* controller = dynamic_cast<FlyCameraController*>(flyController.get());
	if (controller)
	{
		controller->SetHomePosition(Vec3f(4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0));
	}
	
	sceneGroup = new FGroup("SceneData");


	//Ptr<FGeometry> boxGeom = ShapeGeometryBuilder::instance().BuildBox(FBox(Vec3f(1.0,1.0,1.0)));
	//Ptr<FNode> triangle = new FDx11Mesh(boxGeom.get(), m_pDevice);
	//triangle->SetName("triangle");
	//FNode* mesh = triangle.get();
	//// ()是真正执行才能确定的参数  []是只后要处理的对象 
	///*triangle->SetUpdateCallback([mesh](float dt) {
	//	static float theta = 0.0f;
	//	theta += 0.01f * dt;
	//	mesh->SetRotate(theta, 0.0f, 1.0f, 0.0f);
	//});*/
	//sceneGroup->AddChild(triangle);
	GltfReader reader;
	gltfModel = make_shared<GLTFModel>();
	reader.loadAssets("D:\\GitProject\\FEngine\\Assets\\PbrBox\\BoomBox.gltf", *gltfModel.get());
	Ptr<FNode> gltfMesh = new FDx11Mesh(dynamic_cast<FGeometry*>((gltfModel->node).get()), m_pDevice);
	sceneGroup->AddChild(gltfMesh);
	return true;
}

void FDx11App::DrawScene()
{
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	m_pDevice.deviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pDevice.deviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ExecuteMainPass(forwardPass.get());
	sceneGroup->Draw();
	//ExecuteMainPass(skyPass.get());
	//sceneGroup->Draw();

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
		//scale(20.0f,20.0f,20.0f) * rotate(3.14f, Vec3<float>(0.0f,1.0f,0.0f)),
		Vec4f(mainCamera->GetEyePos(),1.0f)
		}
	);

}
