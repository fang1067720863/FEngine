#include "..\Public\FDx11Pipeline.h"
#include"Camera.h"
#include"Group.h"

void FDx11Pipeline::Execute(FGroup* group)
{
	forwardPass->Begin();
	group->Draw();
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
}

void FDx11Pipeline::Init()
{
	_InitAllConstantBuffer();
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

	return;
}

void FDx11Pipeline::_InitForwardPassShaderInput()
{
	FDx11Pass* pass = forwardPass.get();
	CBOPtr frameCBO = ConstantBufferPool::Instance().GetResource("frame");
	CBOPtr resizeCBO = ConstantBufferPool::Instance().GetResource("onResize");
	CBOPtr lightCBO = ConstantBufferPool::Instance().GetResource("light");
	CBOPtr worldCBO = ConstantBufferPool::Instance().GetResource("world");

	pass->GetGpuProgram()->AddConstantBuffer(frameCBO);
	pass->GetGpuProgram()->AddConstantBuffer(resizeCBO);
	pass->GetGpuProgram()->AddConstantBuffer(lightCBO);
	pass->GetGpuProgram()->AddConstantBuffer(worldCBO);
}

void FDx11Pipeline::_InitSkyPassShaderInput()
{
	FDx11Pass* pass = skyPass.get();
	CBOPtr skyMVP = ConstantBufferPool::Instance().GetResource("frame");
	pass->GetGpuProgram()->AddConstantBuffer(skyMVP);
}

void FDx11Pipeline::_InitDeferredPassShaderInput()
{
	FDx11Pass* pass = deferredPass.get();
	CBOPtr defer0 = ConstantBufferPool::Instance().GetResource("frame");
	CBOPtr defer1 = ConstantBufferPool::Instance().GetResource("onResize");
	CBOPtr defer2 = ConstantBufferPool::Instance().GetResource("Light");

	pass->GetGpuProgram()->AddConstantBuffer(defer0);
	pass->GetGpuProgram()->AddConstantBuffer(defer1);
	pass->GetGpuProgram()->AddConstantBuffer(defer2);


	pass->GetGpuProgram()->AddSamplerResource(SamplerStateType::LINIEAR_WRAP, 0);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer0"), 0);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer1"), 1);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBuffer2"), 2);
	pass->GetGpuProgram()->AddShaderResource(pass->GlobalContext()->GetSrvMap("gBufferDepth"), 3);
}

void FDx11Pipeline::_InitGBufferPassShaderInput()
{
	FDx11Pass* pass = gBufferPass.get();
	CBOPtr frameCBO = ConstantBufferPool::Instance().GetResource("frame");
	CBOPtr resizeCBO = ConstantBufferPool::Instance().GetResource("onResize");
	CBOPtr worldCBO = ConstantBufferPool::Instance().GetResource("world");

	pass->GetGpuProgram()->AddConstantBuffer(frameCBO);
	pass->GetGpuProgram()->AddConstantBuffer(resizeCBO);
	pass->GetGpuProgram()->AddConstantBuffer(worldCBO);
}



