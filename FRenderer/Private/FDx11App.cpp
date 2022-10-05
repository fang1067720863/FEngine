#include"FDx11App.h"
#include<D3DX11tex.h>
#include"Shape.h"
#include"GltfReader.h"
#include"FDx11Pipeline.h"


FDx11App::FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: D3DApp(hInstance, windowName, initWidth, initHeight)
{
	

}

bool FDx11App::InitGameObject()
{
	sceneGroup = new FGroup("SceneData");

	mainCamera = new FCamera(Frustum(), "MainCamera");
	CameraManager::Instance().AddResouce(CameraManager::MainCamera, mainCamera);
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
	pipeline->Execute(sceneGroup.get());
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
