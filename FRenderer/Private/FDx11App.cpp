#include"FDx11App.h"

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
	m_pDevice.GetDeviceContext()->IASetInputLayout(pass->GetGpuProgram()->inputLayout.Get());
	// shader & shader resource
	SetGpuProgram(pass->GetGpuProgram());
	// vertex inputlayout
	//evice.GetDeviceContext()->IASetInputLayout(pass->GetInputLayout()->GetD3D11InputLayout());

}

void FDx11App::UpdateScene(float dt)
{
	while (!bufferdEvents.empty())
	{
		Ptr<Event> evt = bufferdEvents.front();
		evt->Handled(*flyController);
		//flyController->Handle(*(evt.get()));
		bufferdEvents.pop_front();
	}
	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::GetInstance().GetConstantBuffer("frame");

	//static float phi = 0.0f, theta = 0.0f;
	//phi += 0.3f * dt, theta += 0.01f * dt;
	Mat4 mat;
	//mat.rotate(theta, 0.0f, 1.0f, 0.0f);

	frameCBO->Upload<CBChangesEveryFrame>(
		CBChangesEveryFrame{
		mainCamera->GetViewMatrix(), mat, Vec4f(mainCamera->GetEyePos(),1.0f)
		}
	);

}

void FDx11App::SetGpuProgram(FDx11GpuProgram* program)
{
	// shader
	m_pDevice.GetDeviceContext()->VSSetShader(program->GetVertexShader(), nullptr, 0);
	m_pDevice.GetDeviceContext()->PSSetShader(program->GetPixelShader(), nullptr, 0);

	// 一个slot 只设置一块resource element ,不设置成Array magicNumber = 1
	// shader resource constant buffer
	unsigned int num = program->GetConstantBufferNum();
	for (unsigned int i = 0; i < num; i++)
	{
		ID3D11Buffer* bufferArray[1];
		ConstantBufferObject* cbo = program->GetContantBufferObject(0);
		bufferArray[0] = cbo->GetBufferView();

		UINT slot = i;
		m_pDevice.GetDeviceContext()->VSSetConstantBuffers(slot, 1, bufferArray);
	}
	ID3D11Buffer* bufferArray[1];
	bufferArray[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("frame")->GetBufferView();
	m_pDevice.GetDeviceContext()->VSSetConstantBuffers(0, 1, bufferArray);
	m_pDevice.GetDeviceContext()->PSSetConstantBuffers(0, 1, bufferArray);
	ID3D11Buffer* bufferArray2[1];
	bufferArray2[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("onResize")->GetBufferView();
	m_pDevice.GetDeviceContext()->VSSetConstantBuffers(1, 1, bufferArray2);
	m_pDevice.GetDeviceContext()->PSSetConstantBuffers(1, 1, bufferArray2);
	ID3D11Buffer* bufferArray3[1];
	bufferArray3[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("light")->GetBufferView();
	m_pDevice.GetDeviceContext()->VSSetConstantBuffers(2, 1, bufferArray3);
	m_pDevice.GetDeviceContext()->PSSetConstantBuffers(2, 1, bufferArray3);
	ID3D11Buffer* bufferArray4[1];
	bufferArray4[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("material")->GetBufferView();
	m_pDevice.GetDeviceContext()->VSSetConstantBuffers(3, 1, bufferArray4);
	m_pDevice.GetDeviceContext()->PSSetConstantBuffers(3, 1, bufferArray4);

	// shader resource texture
	num = program->GetTextureNum();
	for (unsigned int i = 0; i < num; i++)
	{

		UINT slot = i;
		UINT num = 1;
		m_pDevice.GetDeviceContext()->PSSetShaderResources(slot, num, program->GetGpuTextureView(i));
	}

	// shader resource sampler
	num = program->GetSamplerNum();
	for (unsigned int i = 0; i < num; i++)
	{
		UINT slot = i;
		UINT num = 1;
		m_pDevice.GetDeviceContext()->PSSetSamplers(slot, num, program->GetSamplerView(i));
	}

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
void FDx11App::InitMainCamera()
{
	mainCamera = new FCamera(Frustum(), "MainCamera");
	mainCamera->lookAt<float>(Vec3f(-4.0f, 0.0f, 0.0f), Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
	flyController = new FlyCameraController(mainCamera);
}
bool FDx11App::InitSinglePass()
{
	forwardPass = new FDx11Pass(1, m_ScreenViewport,m_pDevice);
	return true;
}

void FDx11App::InitCommmonConstantBuffer()
{

	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("frame", sizeof(CBChangesEveryFrame), m_pDevice);

	frameCBO->Upload<CBChangesEveryFrame>(
		CBChangesEveryFrame{
		mainCamera->GetViewMatrix(), Mat4(), Vec4f(-4.0,0.0,0.0,1.0f)
		}
	);
	Ptr<ConstantBufferObject> resizeCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("onResize", sizeof(CBChangesOnResize), m_pDevice);
	resizeCBO->Upload<CBChangesOnResize>(CBChangesOnResize{ mainCamera->GetProjMatrix() });

	// todo 修改rpi主动修改rhi
	Ptr<ConstantBufferObject> lightCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("light", sizeof(Light), m_pDevice);
	lightCBO->Upload<Light>(Light{ Vec4f(1.0,1.0,1.0,1.0),Vec4f(0.8,0.8,0.8,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.0,1.0) });

	Ptr<ConstantBufferObject> materialCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("material", sizeof(Material), m_pDevice);
	materialCBO->Upload<Material>(Material{ Vec4f(0.1,0.1,0.1,1.0),Vec4f(0.2,0.2,0.2,1.0),Vec4f(0.7,0.7,0.7,1.0),Vec4f(0.5,0.5,0.5,1.0) });
	
}
void FDx11App::DrawScene()
{

	//UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	//UINT offset = 0;						// 起始偏移量
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	//m_pd3dDevice
	m_pDevice.deviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pDevice.deviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ExecuteMainPass(forwardPass.get());


	//m_pDevice.deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	//m_pDevice.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_pDevice.deviceContext->IASetInputLayout(m_pVertexLayout.Get());
	//m_pDevice.deviceContext->Draw(3, 0);
	triangleMesh->Draw();

	HR(m_pSwapChain->Present(0, 0));
}

bool FDx11App::InitResource()
{
	InitMainCamera();
	Ptr<FGeometry> boxGeom = ShapeGeometryBuilder::instance().BuildBox(FBox(Vec3f(1.0,1.0,1.0)));
	triangleMesh = new FDx11Mesh(boxGeom.get(), m_pDevice);


	return true;
}
