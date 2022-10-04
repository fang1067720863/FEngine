#include"FDx11App.h"
#include<D3DX11tex.h>
#include"Shape.h"
#include"GltfReader.h"




FDx11App::FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: D3DApp(hInstance, windowName, initWidth, initHeight)
{
	PassBuilder::Instance().SetDevice(&m_pDevice);
	PassBuilder::Instance().context->mainScreenViewport = m_ScreenViewport;

}

bool FDx11App::InitSinglePass()
{
	PassBuilder::PassOption option;
	option.name = "forwardPass";
	forwardPass = PassBuilder::Instance().CreatePass(option);
	forwardPass->InitPass("1DefaultVertex", "1PbrPS");
	_InitForwardPassShaderInput();

	
	option.name = "skyPass";
	option.mainTarget = true;
	option.numViews = 1;
	option.stateset.dst = DepthStencilStateType::DRAW_WITH_STENCIL;
	skyPass = PassBuilder::Instance().CreatePass(option);
	skyPass->InitPass("2skybox_vs", "2skybox_ps");
	_InitSkyPassShaderInput();


	option.name = "gBuffer";
	option.mainTarget = false;
	option.numViews = 3;
	option.stateset.dst = DepthStencilStateType::LESS_EQUAL;
	gBufferPass = PassBuilder::Instance().CreatePass(option);
	gBufferPass->InitPass("3GBufferVS", "3GBufferPS");
	_InitGBufferPassShaderInput();

	option.name = "deferred";
	option.mainTarget = true;
	option.numViews = 1;
	option.stateset.dst = DepthStencilStateType::NO_DEPTH_TEST_WITH_STENCIL;
	deferredPass = PassBuilder::Instance().CreatePass(option);
	deferredPass->InitPass("3DeferredVS", "3DeferredPS");
	_InitDeferredPassShaderInput();

	return true;
}
void FDx11App::_InitAllPassShaderInput()

{

}
void FDx11App::_InitGBufferPassShaderInput()
{
	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::Instance().GetResource("frame");
	Ptr<ConstantBufferObject> resizeCBO = ConstantBufferPool::Instance().GetResource("onResize");
	Ptr<ConstantBufferObject> worldCBO = ConstantBufferPool::Instance().GetResource("world");

	gBufferPass->GetGpuProgram()->AddConstantBuffer(frameCBO);
	gBufferPass->GetGpuProgram()->AddConstantBuffer(resizeCBO);
	gBufferPass->GetGpuProgram()->AddConstantBuffer(worldCBO);
}
void FDx11App::_InitDeferredPassShaderInput()
{
	Ptr<ConstantBufferObject> defer0 = ConstantBufferPool::Instance().CreateDeviceResource("defer0", sizeof(CBDeferred0), m_pDevice);
	Ptr<ConstantBufferObject> defer1 = ConstantBufferPool::Instance().CreateDeviceResource("defer1", sizeof(CBDeferred1), m_pDevice);
	
	mainCamera->AddViewUpdateCallback([defer0, camera = this->mainCamera](float dt)
	{
		defer0->Upload<CBDeferred0>(
			CBDeferred0{
				camera->GetViewMatrix(), camera->GetViewMatrixInverse(), Vec4f(camera->GetEyePos(),1.0)
			});
	});
	mainCamera->AddProjUpdateCallback([defer1, camera = this->mainCamera](float dt)
	{
		defer1->Upload<CBDeferred1>(
			CBDeferred1{
				camera->GetProjMatrix(), camera->GetProjMatrixInverse(),camera->GetZNear(),camera->GetZFar(), Vec2f(0.0f,0.0f)
			}
		);
	});
	Ptr<ConstantBufferObject> defer2 = ConstantBufferPool::Instance().CreateDeviceResource("defer2", sizeof(Light), m_pDevice);
	defer2->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });
	


	deferredPass->GetGpuProgram()->AddConstantBuffer(defer0);
	deferredPass->GetGpuProgram()->AddConstantBuffer(defer1);
	deferredPass->GetGpuProgram()->AddConstantBuffer(defer2);


	deferredPass->GetGpuProgram()->AddSamplerResource(SamplerStateType::LINIEAR_WRAP,0);

	deferredPass->GetGpuProgram()->AddShaderResource(deferredPass->GlobalContext()->GetSrvMap("gBuffer0"), 0);
	deferredPass->GetGpuProgram()->AddShaderResource(deferredPass->GlobalContext()->GetSrvMap("gBuffer1"), 1);
	deferredPass->GetGpuProgram()->AddShaderResource(deferredPass->GlobalContext()->GetSrvMap("gBuffer2"), 2);
	deferredPass->GetGpuProgram()->AddShaderResource(deferredPass->GlobalContext()->GetSrvMap("gBufferDepth"), 3);



}
void FDx11App::_InitSkyPassShaderInput()
{

	Ptr<ConstantBufferObject> skyMVP = ConstantBufferPool::Instance().CreateDeviceResource("skyMVP", sizeof(Mat4), m_pDevice);
	skyPass->GetGpuProgram()->AddConstantBuffer(skyMVP);

	mainCamera->AddViewUpdateCallback([skyMVP, camera = this->mainCamera](float dt)
	{
		Mat4 V = camera->GetViewMatrix();
		Mat4 VP{
			rotate(V.GetRotate()) * camera->GetProjMatrix()
		};
		VP.transpose();
		skyMVP->Upload<Mat4>(VP);
	});

}
void FDx11App::_InitForwardPassShaderInput()
{
	
	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::Instance().CreateDeviceResource("frame", sizeof(CBChangesEveryFrame), m_pDevice);
	Ptr<ConstantBufferObject> resizeCBO = ConstantBufferPool::Instance().CreateDeviceResource("onResize", sizeof(CBChangesOnResize), m_pDevice);

	// todo 修改rpi主动修改rhi
	Ptr<ConstantBufferObject> lightCBO = ConstantBufferPool::Instance().CreateDeviceResource("light", sizeof(Light), m_pDevice);
	lightCBO->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });

	Ptr<ConstantBufferObject> worldCBO = ConstantBufferPool::Instance().CreateDeviceResource("world", sizeof(CBEveryObject), m_pDevice);
	worldCBO->Upload<CBEveryObject>(CBEveryObject{ Mat4() });


	forwardPass->GetGpuProgram()->AddConstantBuffer(frameCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(resizeCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(lightCBO);
	forwardPass->GetGpuProgram()->AddConstantBuffer(worldCBO);



	mainCamera->AddViewUpdateCallback([frameCBO, camera = this->mainCamera](float dt)
		{
			frameCBO->Upload<CBChangesEveryFrame>(
				CBChangesEveryFrame{
				camera->GetViewMatrix(),
				Vec4f(camera->GetEyePos(),1.0f)
				});
		});

	mainCamera->AddProjUpdateCallback([resizeCBO, camera = this->mainCamera](float dt)
	{
	
		resizeCBO->Upload<CBChangesOnResize>(CBChangesOnResize{
			camera->GetProjMatrix() }
			);
	});
	

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
	Ptr<FGeometry> skyboxGeom = ShapeGeometryBuilder::instance().BuildGeomtry(box);
	skyboxGeom->SetMaterialType(MaterialType::SkyBox);
	skyboxGeom->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model"));
	skybox = new FDx11Mesh(skyboxGeom.get(), m_pDevice);


	Ptr<FGeometry> quad = ShapeGeometryBuilder::instance().BuildGeomtry(FQuad());
	quad->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model"));
	renderQuad = new FDx11Mesh(quad.get(), m_pDevice);

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


	
	forwardPass->Begin();
	sceneGroup->Draw();
	forwardPass->End();

	//gBufferPass->Begin();
	//sceneGroup->Draw();
	//gBufferPass->End();

	//deferredPass->Begin();
	//renderQuad->Draw();
	//deferredPass->End();

	//skyPass->Begin();
	//skybox->Draw();
	//skyPass->End();

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
	mainCamera->Update(dt);




}
