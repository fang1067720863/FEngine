#include"FDx11App.h"
#include<D3DX11tex.h>
#include"Shape.h"
#include"Light.h"
#include"GltfReader.h"
#include"FDx11Pipeline.h"
#include"CameraManipulator.h"


FDx11App::FDx11App(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
	: D3DApp(hInstance, windowName, initWidth, initHeight)
{
	

}

bool FDx11App::Init()
{
	if (!D3DApp::Init())
		return false;

	FindGlobalPath();
	if (!InitGameObject())
		return false;

	D3DApp::PostInit();

	return true;
}
	
bool FDx11App::InitGameObject()
{
	sceneGroup = new FGroup("SceneData");

	mainCamera = new FCamera(Frustum(), "MainCamera");
	CameraManager::Instance().AddResouce(CameraManager::MainCamera, mainCamera);
	/*Ptr<EventHandler>flyController = new FlyCameraController(mainCamera);
	controllers.push_back(flyController);
	FlyCameraController* controller = dynamic_cast<FlyCameraController*>(flyController.get());
	if (controller)
	{
		controller->SetHomePosition(Vec3f(4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0));
	}*/

	Ptr<DirectLight> defaultLight = LightManager::Instance().CreateDirectLight();
	defaultLight->direction = Vec4f(0.0, 1.0, 0.0, 0.0);

	FBox box(Vec3f(1.0f));
	Ptr<FGeometry> skyboxGeom = ShapeGeometryBuilder::instance().BuildGeomtry(box);
	skyboxGeom->SetMaterialType(MaterialType::SkyBox);
	skyboxGeom->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model"));
	skybox = new FDx11Mesh(skyboxGeom.get(), m_pDevice);


	Ptr<FGeometry> quad = ShapeGeometryBuilder::instance().BuildGeomtry(FQuad());
	quad->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model"));

	renderQuad = new FDx11Mesh(quad.get(), m_pDevice);
	//renderQuad->SetRenderMask(static_cast<uint16_t>(RenderMask::DeferQuad));

	Ptr<FGeometry> land = ShapeGeometryBuilder::instance().BuildGeomtry(FQuad());
	land->SetModelFile(File("D:\\GitProject\\FEngine\\FRenderer\\Model\\"));
	land->SetScale(Vec3f(20.0f));
	land->SetMaterialType(MaterialType::Pbr);
	MaterialPtr material = make_shared<PbrMaterialMetalRoughness>();
	MaterialMapPtr materialMap = make_shared<MaterialMap>();
	int32_t slot = MaterialBuilder::Instance().CreateResouce(material);
	int32_t mapSlot = MaterialMapBuilder::Instance().CreateResouce(materialMap);

	//material->baseColorFactor = glTFMaterial.pbrMetallicRoughness.baseColorFactor;
	//material->metallicFactor = static_cast<float>(glTFMaterial.pbrMetallicRoughness.metallicFactor);
	//material->roughnessFactor = static_cast<float>(glTFMaterial.pbrMetallicRoughness.roughnessFactor);
	//material->alphaMaskCutoff = static_cast<float>(glTFMaterial.alphaCutoff);
	//material->aoStrength = static_cast<float>(glTFMaterial.occlusionTexture.strength);

	materialMap->baseColorMap.uri = "TexturesCom_Brick_BlocksBare_1K_albedo.tif";
	materialMap->metalRoughnessMap.uri = "TexturesCom_Brick_BlocksBare_1K_roughness.tif";
	materialMap->normalMap.uri = "TexturesCom_Brick_BlocksBare_1K_normal.tif";
	materialMap->emissiveMap.uri = "TexturesCom_Brick_BlocksBare_1K_normal.tif";
	materialMap->aoMap.uri = "TexturesCom_Brick_BlocksBare_1K_ao.tif";

	land->materialMapSlot = mapSlot;
	land->materialSlot = slot;

	Ptr<FNode> landMesh= new FDx11Mesh(land.get(), m_pDevice);
	landMesh->SetScale(Vec3f(10.0f));
	landMesh->SetPosition(Vec3f(0.0f,-6.0f,0.0f));
	landMesh->SetRotate(1.57f, 1.0f, 0.0f, 0.0f);
	landMesh->SetRenderMask(static_cast<uint16_t>(RenderMask::Forward));
	sceneGroup->AddChild(landMesh);

	GltfReader reader;
	gltfModel = make_shared<GLTFModel>();
	reader.loadAssets("D:\\GitProject\\FEngine\\Assets\\PbrBox\\BoomBox.gltf", *gltfModel.get());
	Ptr<FNode> gltfMesh = new FDx11Mesh(dynamic_cast<FGeometry*>((gltfModel->node).get()), m_pDevice);
	gltfMesh->SetScale(Vec3f(25.0f));
	gltfMesh->SetPosition(Vec3f(0.0f,0.0f,0.0f));
	sceneGroup->AddChild(gltfMesh); 

	Ptr<EventHandler> pawnCtrl = new PawnController(gltfMesh);
	controllers.push_back(pawnCtrl);

	gltfMesh->SetUpdateCallback(
		[gltfMesh](float dt) {
				static float theta = 0.0f;
				theta += 0.005f * dt;
				gltfMesh->SetRotate(theta, 0.0f, 1.0f, 0.0f);
		});
	
	Ptr<EventHandler>flyController = new TpsCameraController(mainCamera, gltfMesh);
	controllers.push_back(flyController);
	
	
	return true;
}

void FDx11App::UpdateScene(float dt)
{
	// handle events
	while (!eventQueue.Empty())
	{
		Ptr<Event> evt = eventQueue.TakeoutEvent();
		for (auto& controller : controllers)
		{
			evt->Handled(*controller);
		}
		for (auto& controller : controllers)
		{
			controller->Update();
		}
	}
	sceneGroup->Update(dt);
	mainCamera->Update(dt);
}

void FDx11App::DrawScene()
{
	pipeline->Execute(sceneGroup.get());
	HR(m_pSwapChain->Present(0, 0));
}
