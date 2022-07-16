#include"FDx11App.h"

void FDx11App::ExecuteMainPass(FDx11Pass* pass)
{

	// Set Viewport
	//device.GetDeviceContext()->RSSetViewports(1, &m_ScreenViewport);
	//device.GetDeviceContext()->RSSetScissorRects(1, pass->GetScissorRects());

	// Clear&Set FrameBuffer

	//device.GetDeviceContext()->OMSetRenderTargets(1, pass->mMainRTV.GetAddressOf(), pass->GetDepthStencilView());
	m_pDevice.GetDeviceContext()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	// Render state
	//SetRenderStates(gBufferPass->GetRenderStates());
	//SetRenderStates();
	//m_pDevice.GetDeviceContext()->IASetInputLayout(pass->GetGpuProgram()->inputLayout.Get());
	// shader & shader resource
	SetGpuProgram(pass->GetGpuProgram());
	// vertex inputlayout
	//evice.GetDeviceContext()->IASetInputLayout(pass->GetInputLayout()->GetD3D11InputLayout());

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
	//ID3D11Buffer* bufferArray[1];
	//bufferArray[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("frame")->GetBufferView();
	//device.GetDeviceContext()->VSSetConstantBuffers(0, 1, bufferArray);
	//ID3D11Buffer* bufferArray2[1];
	//bufferArray2[0] = ConstantBufferPool::GetInstance().GetConstantBuffer("onResize")->GetBufferView();
	//device.GetDeviceContext()->VSSetConstantBuffers(1, 1, bufferArray2);

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
}
bool FDx11App::InitSinglePass()
{
	forwardPass = new FDx11Pass(1, m_ScreenViewport,m_pDevice);
	return true;
}
//
//void  FDx11App::InitGameObject()
//{
//
//	FBox box(2.0);
//	/*Ptr<FGeometry> sphereGeom = ShapeGeometryBuilder::instance().BmSwapChain->PresentuildBox(box);
//	sphereMesh = new FDx11Mesh(sphereGeom.get(),device);
//	sphereMesh->SetLocalPosition(Vec3f(0.0, 1.0, 0.0));*/
//	Ptr<FGeometry> triangleGeom = ShapeGeometryBuilder::instance().BuildTriangle();
//	triangleMesh = new FDx11Mesh(triangleGeom.get(), device, false);
//	triangleMesh->mUseIndex = false;
//	triangleMesh->SetLocalPosition(Vec3f(0.0, 0.0, 0.0));
//}
//
//void FDx11App::InitCommmonConstantBuffer()
//{
//
//	//ConstantBufferObject *frame = new ConstantBufferObject(sizeof(CBChangesEveryFrame), device);
//
//	Ptr<ConstantBufferObject> frameCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("frame", sizeof(CBChangesEveryFrame), device);
//
//	frameCBO->Upload<CBChangesEveryFrame>(
//		CBChangesEveryFrame{
//		mainCamera->GetViewMatrix(), Mat4(), Vec4f(-4.0,0.0,0.0,1.0f)
//		}
//	);
//	Ptr<ConstantBufferObject> resizeCBO = ConstantBufferPool::GetInstance().CreateConstantBuffer("onResize", sizeof(CBChangesOnResize), device);
//	resizeCBO->Upload<CBChangesOnResize>(CBChangesOnResize{ mainCamera->GetProjMatrix() });
//
//	/*Ptr<UniformDefault> dirLight = new UniformDefault("dirLight", {});
//	UniformBasePtr diffuse = new UniformVec4f("diffuse", Vec4f(0.8f, 0.8f, 0.8f, 1.0f));
//	UniformBasePtr specular = new UniformVec4f("specular", Vec4f(0.1f, 0.1f, 0.1f, 1.0f));
//	UniformBasePtr direction = new UniformVec4f("direction", Vec4f(0.577f, -0.577f, -0.577f, 1.0f));
//	UniformBasePtr ambient = new UniformVec4f("direction", Vec4f(0.8f, 0.8f, 0.8f, 1.0f));
//	dirLight->AddSubUniform(diffuse);
//	dirLight->AddSubUniform(specular);
//	dirLight->AddSubUniform(direction);
//	dirLight->AddSubUniform(ambient);*/
//
//	//m_BasicEffect.SetDirLight(dirLight);
//	//m_BasicEffect.SetEyePos(m_pCamera->GetPosition());
//	//m_BasicEffect.SetProjMatrix(m_pCamera->GetProjXM());
//	//m_BasicEffect.SetViewMatrix(m_pCamera->GetViewXM());
//	//m_BasicEffect.SetRenderDefault(m_pd3dImmediateContext.Get());
//	//m_BasicEffect.InitAll(m_pd3dDevice.Get());
//	//struct CBChangesEveryObjectDrawing
//	//{
//	//	Material material;
//	//};
//
//
//
//}
void FDx11App::DrawScene()
{

	UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量
	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
	//m_pd3dDevice
	m_pDevice.deviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pDevice.deviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//ExecuteMainPass(forwardPass.get());
	m_pDevice.deviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDevice.deviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pDevice.deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	// 设置图元类型，设定输入布局
	m_pDevice.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevice.deviceContext->IASetInputLayout(m_pVertexLayout.Get());
	// 绘制三角形
	m_pDevice.deviceContext->Draw(3, 0);
	HR(m_pSwapChain->Present(0, 0));
}
bool FDx11App::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器
	HR(CreateShaderFromFile(L"Shader\\Triangle_VS2.cso", L"Shader\\Triangle_VS2.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pDevice.device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
	// 创建并绑定顶点布局
	HR(m_pDevice.device->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

	// 创建像素着色器
	HR(CreateShaderFromFile(L"Shader\\Triangle_PS2.cso", L"Shader\\Triangle_PS2.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pDevice.device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

	return true;
}
const D3D11_INPUT_ELEMENT_DESC FDx11App::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
bool FDx11App::InitResource()
{
	// 设置三角形顶点
	VertexPosColor vertices[] =
	{
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
	};
	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	HR(m_pDevice.device->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	//

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量

	
	m_pDevice.deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	// 设置图元类型，设定输入布局
	m_pDevice.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevice.deviceContext->IASetInputLayout(m_pVertexLayout.Get());
	// 将着色器绑定到渲染管线
	m_pDevice.deviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pDevice.deviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// ******************
	// 设置调试对象名
	//
	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
	D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");



	return true;
}


//void FDx11App::Draw()
//{
//	/*ExecuteMainPass(singlePass.get());*/
//	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA = (0,0,0,255)
//	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
//	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//	DrawScene();
//	//triangleMesh->Draw();
//	//sphereMesh->Draw();
//	//mSwapChain->Present(0, 0);
//}
//
//void FDx11App::Update()
//{
//	FDx11Renderer::Update();
//}
