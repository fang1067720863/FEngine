#include "..\Public\FDx11Pipeline.h"
#include"Camera.h"
#include"Group.h"
#include"Light.h"

void FDx11Pipeline::Execute(FGroup* group)
{
	shadowPass->Begin();
	group->Draw(static_cast<uint16_t>(RenderMask::Shadow));
	shadowPass->End();
	forwardPass->Begin();
	group->Draw(static_cast<uint16_t>(RenderMask::Forward));
	forwardPass->End();
}

void FDx11Pipeline::_InitAllConstantBuffer()
{
	// forward
	CBOPtr frameCBO = ConstantBufferPool::Instance().CreateDeviceResource("frame", sizeof(CBChangesEveryFrame), device);
	CBOPtr resizeCBO = ConstantBufferPool::Instance().CreateDeviceResource("onResize", sizeof(CBChangesOnResize), device);
	CBOPtr lightCBO = ConstantBufferPool::Instance().CreateDeviceResource("light", sizeof(Light), device);
	CBOPtr worldCBO = ConstantBufferPool::Instance().CreateDeviceResource("world", sizeof(CBEveryObject), device);
	
	// gBuffer
	CBOPtr skyMVP = ConstantBufferPool::Instance().CreateDeviceResource("skyMVP", sizeof(Mat4), device);
	// defer
	CBOPtr defer0 = ConstantBufferPool::Instance().CreateDeviceResource("defer0", sizeof(CBDeferred0), device);
	CBOPtr defer1 = ConstantBufferPool::Instance().CreateDeviceResource("defer1", sizeof(CBDeferred1), device);
	CBOPtr defer2 = ConstantBufferPool::Instance().CreateDeviceResource("defer2", sizeof(Light), device);

	// shadow

	CBOPtr shadow0 = ConstantBufferPool::Instance().CreateDeviceResource("shadow0", sizeof(CBShadowLightView), device);
	CBOPtr shadow1 = ConstantBufferPool::Instance().CreateDeviceResource("shadow1", sizeof(CBShadowLightProj), device);

	CBOPtr lightTransform = ConstantBufferPool::Instance().CreateDeviceResource("lightTransform", sizeof(Mat4), device);



	FCamera * mainCamera = CameraManager::Instance().GetResource(CameraManager::MainCamera).get();
	mainCamera->AddViewUpdateCallback([defer0, skyMVP, camera = mainCamera](float dt)
	{
			Mat4 V = camera->GetViewMatrix(); 
			Mat4 VP{
				rotate(V.GetRotate()) * camera->GetProjMatrix()
			};
			VP.transpose();
			skyMVP->Upload<Mat4>(VP);

		defer0->Upload<CBDeferred0>(
			CBDeferred0{
				camera->GetViewMatrix(), camera->GetViewMatrixInverse(), Vec4f(camera->GetEyePos(),1.0)
			});
	});
	mainCamera->AddProjUpdateCallback([defer1, camera = mainCamera](float dt)
	{
		defer1->Upload<CBDeferred1>(
			CBDeferred1{
				camera->GetProjMatrix(), camera->GetProjMatrixInverse(),camera->GetZNear(),camera->GetZFar(), Vec2f(0.0f,0.0f)
			}
		);
	});

	mainCamera->AddViewUpdateCallback([frameCBO, camera = mainCamera](float dt)
	{
		frameCBO->Upload<CBChangesEveryFrame>(
			CBChangesEveryFrame{
			camera->GetViewMatrix(),
			Vec4f(camera->GetEyePos(),1.0f)
			});
	});

	mainCamera->AddProjUpdateCallback([resizeCBO, camera = mainCamera](float dt)
	{
		resizeCBO->Upload<CBChangesOnResize>(CBChangesOnResize{
			camera->GetProjMatrix() }
		);
	});

	
	// default Value
	defer2->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });
	lightCBO->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });
	worldCBO->Upload<CBEveryObject>(CBEveryObject{ Mat4() });

	Ptr<DirectLight> light = LightManager::Instance().GetDirectLight0();
	
	Vec3f lightLoc = light->direction.asVec3() * (2.0f);
	lightLoc = Vec3f(4.0f, 4.0f, 0.0f);
	Mat4 view(Mat4::lookAt(Vec3(0.0f, 1.0f, 0.0f), lightLoc, Vec3f(0.0f)));
	view.transpose();
	shadow0->Upload<CBShadowLightView>(CBShadowLightView{ view });

     Mat4 proj(Mat4::perspective(3.1415f / 3, 4.0f / 3, 0.5f, 500.0f));
	//Mat4 proj(Mat4::orthographic(0.0f, 10.0f, 0.0f, 10.0f, 2.0f, 60.0f));
	shadow1->Upload<CBShadowLightProj>(CBShadowLightProj{proj});
	lightTransform->Upload<Mat4>(proj * view);



}

void FDx11Pipeline::Init()
{
	_InitAllConstantBuffer();

	PassBuilder::PassOption option;
	option.name = "shadowMap";
	option.mainTarget = false;
	option.numViews = 1;
	option.stateset.dst = DepthStencilStateType::LESS_EQUAL;
	shadowPass = PassBuilder::Instance().CreatePass(option);
	std::vector<std::string> shadowParameter{ "shadow0","shadow1", "world" };
	shadowPass->SetConstantBufferSlots(std::move(shadowParameter));
	shadowPass->InitPass("4ShadowVS", "4ShadowPS");

	PassBuilder::PassOption option1;
	option1.name = "forwardPass";
	forwardPass = PassBuilder::Instance().CreatePass(option1);
	std::vector<std::string> forwardParameter{ "frame","onResize","light","world" , "lightTransform"};
	forwardPass->SetConstantBufferSlots(std::move(forwardParameter));
	forwardPass->InitPass("1DefaultVertex", "1PbrPS");
	_InitForwardPassShaderInput();

	PassBuilder::PassOption option2;
	option2.name = "skyPass";
	option2.mainTarget = true;
	option2.stateset.dst = DepthStencilStateType::DRAW_WITH_STENCIL;
	skyPass = PassBuilder::Instance().CreatePass(option2);
	std::vector<std::string> skyParameter{"skyMVP"};
	skyPass->SetConstantBufferSlots(std::move(skyParameter));
	skyPass->InitPass("2skybox_vs", "2skybox_ps");

	PassBuilder::PassOption option3;
	option3.name = "gBuffer";
	option3.mainTarget = false;
	option3.numViews = 3;
	option3.stateset.dst = DepthStencilStateType::LESS_EQUAL;
	gBufferPass = PassBuilder::Instance().CreatePass(option3);
	std::vector<std::string> gBufferParameter{ "frame","onResize", "world" };
	gBufferPass->SetConstantBufferSlots(std::move(gBufferParameter));
	gBufferPass->InitPass("3GBufferVS", "3GBufferPS");

	PassBuilder::PassOption option4;
	option4.name = "deferred";
	option4.mainTarget = true;
	option4.stateset.dst = DepthStencilStateType::NO_DEPTH_TEST_WITH_STENCIL;
	deferredPass = PassBuilder::Instance().CreatePass(option4);
	std::vector<std::string> deferredParameter{ "frame","onResize", "Light" };
	deferredPass->SetConstantBufferSlots(std::move(deferredParameter));
	deferredPass->InitPass("3DeferredVS", "3DeferredPS");
	_InitDeferredPassShaderInput();

	return;
}




void FDx11Pipeline::_InitDeferredPassShaderInput()
{
	FDx11Pass* pass = deferredPass.get();

	pass->GetGpuProgram()->AddSamplerResource(SamplerStateType::LINIEAR_WRAP, 0);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer0"), 0);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer1"), 1);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer2"), 2);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBufferDepth"), 3);
}

void FDx11Pipeline::_InitForwardPassShaderInput()
{
	FDx11Pass* pass = forwardPass.get();
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("shadowMapDepth"), 5);
}




