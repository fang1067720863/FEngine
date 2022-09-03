//
//#include"FDx11Renderer.h"
//#include"FDx11RenderState.h"
//#include"FDx11ResourceFactory.h"
////#include<D3Dcommon.h>
//
//
//
//ComPtr<ID3D11RasterizerState> RenderStates::RSNoCull = nullptr;
//ComPtr<ID3D11RasterizerState> RenderStates::RSWireframe = nullptr;
//ComPtr<ID3D11RasterizerState> RenderStates::RSCullClockWise = nullptr;
//
//ComPtr<ID3D11SamplerState> RenderStates::SSAnistropicWrap = nullptr;
//ComPtr<ID3D11SamplerState> RenderStates::SSLinearWrap = nullptr;
//
//ComPtr<ID3D11BlendState> RenderStates::BSAlphaToCoverage = nullptr;
//ComPtr<ID3D11BlendState> RenderStates::BSNoColorWrite = nullptr;
//ComPtr<ID3D11BlendState> RenderStates::BSTransparent = nullptr;
//ComPtr<ID3D11BlendState> RenderStates::BSAdditive = nullptr;
//
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSLessEqual = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSWriteStencil = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSDrawWithStencil = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDoubleBlend = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthTest = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthWrite = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthTestWithStencil = nullptr;
//ComPtr<ID3D11DepthStencilState> RenderStates::DSSNoDepthWriteWithStencil = nullptr;
//
//bool RenderStates::IsInit()
//{
//	return RSWireframe != nullptr;
//}
//
//void RenderStates::InitAll(ID3D11Device* device)
//{
//	// 先前初始化过的话就没必要重来了
//	if (IsInit())
//		return;
//	// ******************
//	// 初始化光栅化器状态
//	//
//	D3D11_RASTERIZER_DESC rasterizerDesc;
//	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
//
//	// 线框模式
//	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
//	rasterizerDesc.FrontCounterClockwise = false;
//	rasterizerDesc.DepthClipEnable = true;
//	HR(device->CreateRasterizerState(&rasterizerDesc, RSWireframe.GetAddressOf()));
//
//	// 无背面剔除模式
//	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
//	rasterizerDesc.FrontCounterClockwise = false;
//	rasterizerDesc.DepthClipEnable = true;
//	HR(device->CreateRasterizerState(&rasterizerDesc, RSNoCull.GetAddressOf()));
//
//	// 顺时针剔除模式
//	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
//	rasterizerDesc.CullMode = D3D11_CULL_BACK;
//	rasterizerDesc.FrontCounterClockwise = true;
//	rasterizerDesc.DepthClipEnable = true;
//	HR(device->CreateRasterizerState(&rasterizerDesc, RSCullClockWise.GetAddressOf()));
//
//	// ******************
//	// 初始化采样器状态
//	//
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//
//	// 线性过滤模式
//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	HR(device->CreateSamplerState(&sampDesc, SSLinearWrap.GetAddressOf()));
//
//	// 各向异性过滤模式
//	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MaxAnisotropy = 4;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	HR(device->CreateSamplerState(&sampDesc, SSAnistropicWrap.GetAddressOf()));
//
//	// ******************
//	// 初始化混合状态
//	//
//	D3D11_BLEND_DESC blendDesc;
//	ZeroMemory(&blendDesc, sizeof(blendDesc));
//	auto& rtDesc = blendDesc.RenderTarget[0];
//	// Alpha-To-Coverage模式
//	blendDesc.AlphaToCoverageEnable = true;
//	blendDesc.IndependentBlendEnable = false;
//	rtDesc.BlendEnable = false;
//	rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//	HR(device->CreateBlendState(&blendDesc, BSAlphaToCoverage.GetAddressOf()));
//
//	// 透明混合模式
//	// Color = SrcAlpha * SrcColor + (1 - SrcAlpha) * DestColor 
//	// Alpha = SrcAlpha
//	blendDesc.AlphaToCoverageEnable = false;
//	blendDesc.IndependentBlendEnable = false;
//	rtDesc.BlendEnable = true;
//	rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
//	rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
//	rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
//	rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
//	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//
//	HR(device->CreateBlendState(&blendDesc, BSTransparent.GetAddressOf()));
//
//	// 加法混合模式
//	// Color = SrcColor + DestColor
//	// Alpha = SrcAlpha
//	rtDesc.SrcBlend = D3D11_BLEND_ONE;
//	rtDesc.DestBlend = D3D11_BLEND_ONE;
//	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
//	rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
//	rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
//	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//
//	HR(device->CreateBlendState(&blendDesc, BSAdditive.GetAddressOf()));
//
//	// 无颜色写入混合模式
//	// Color = DestColor
//	// Alpha = DestAlpha
//	rtDesc.BlendEnable = false;
//	rtDesc.SrcBlend = D3D11_BLEND_ZERO;
//	rtDesc.DestBlend = D3D11_BLEND_ONE;
//	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
//	rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
//	rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;
//	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	rtDesc.RenderTargetWriteMask = 0;
//	HR(device->CreateBlendState(&blendDesc, BSNoColorWrite.GetAddressOf()));
//
//	// ******************
//	// 初始化深度/模板状态
//	//
//	D3D11_DEPTH_STENCIL_DESC dsDesc;
//
//	// 允许使用深度值一致的像素进行替换的深度/模板状态
//	// 该状态用于绘制天空盒，因为深度值为1.0时默认无法通过深度测试
//	dsDesc.DepthEnable = true;
//	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
//
//	dsDesc.StencilEnable = false;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSLessEqual.GetAddressOf()));
//
//	// 写入模板值的深度/模板状态
//	// 这里不写入深度信息
//	// 无论是正面还是背面，原来指定的区域的模板值都会被写入StencilRef
//	dsDesc.DepthEnable = true;
//	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//	dsDesc.StencilEnable = true;
//	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
//	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
//
//	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
//	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
//	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
//	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));
//
//	// 对指定模板值进行绘制的深度/模板状态
//	// 对满足模板值条件的区域才进行绘制，并更新深度
//	dsDesc.DepthEnable = true;
//	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//	dsDesc.StencilEnable = true;
//	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
//	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
//
//	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//	// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
//	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSDrawWithStencil.GetAddressOf()));
//
//	// 无二次混合深度/模板状态
//	// 允许默认深度测试
//	// 通过自递增使得原来StencilRef的值只能使用一次，实现仅一次混合
//	dsDesc.DepthEnable = true;
//	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//
//	dsDesc.StencilEnable = true;
//	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
//	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
//
//	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
//	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//	// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
//	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
//	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDoubleBlend.GetAddressOf()));
//
//	// 关闭深度测试的深度/模板状态
//	// 若绘制非透明物体，务必严格按照绘制顺序
//	// 绘制透明物体则不需要担心绘制顺序
//	// 而默认情况下模板测试就是关闭的
//	dsDesc.DepthEnable = false;
//	dsDesc.StencilEnable = false;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthTest.GetAddressOf()));
//
//
//	// 关闭深度测试
//	// 若绘制非透明物体，务必严格按照绘制顺序
//	// 绘制透明物体则不需要担心绘制顺序
//	// 对满足模板值条件的区域才进行绘制
//	dsDesc.StencilEnable = true;
//	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
//	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
//
//	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//	// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
//	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthTestWithStencil.GetAddressOf()));
//
//	// 进行深度测试，但不写入深度值的状态
//	// 若绘制非透明物体时，应使用默认状态
//	// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
//	// 并且确保较前的非透明物体可以阻挡较后的一切物体
//	dsDesc.DepthEnable = true;
//	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
//	dsDesc.StencilEnable = false;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthWrite.GetAddressOf()));
//
//
//	// 进行深度测试，但不写入深度值的状态
//	// 若绘制非透明物体时，应使用默认状态
//	// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
//	// 并且确保较前的非透明物体可以阻挡较后的一切物体
//	// 对满足模板值条件的区域才进行绘制
//	dsDesc.StencilEnable = true;
//	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
//	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
//
//	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//	// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
//	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
//
//	HR(device->CreateDepthStencilState(&dsDesc, DSSNoDepthWriteWithStencil.GetAddressOf()));
//
//	// ******************
//}
//
//
//FDx11Renderer::FDx11Renderer(FGraphicContext::Traits* traits, HWND hwnd):mTraits(traits),
//mHwnd(hwnd)
//{
//	
//}
//
//
//void FDx11Renderer::Prepare()
//{
//	
//	InitDirect3D(mTraits, mHwnd);
//	InitSamplerResourcePool();
//	ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));
//	m_ScreenViewport.TopLeftX = 0;
//	m_ScreenViewport.TopLeftY = 0;
//	m_ScreenViewport.Width = static_cast<float>(mClientWidth);
//	m_ScreenViewport.Height = static_cast<float>(mClientHeight);
//	m_ScreenViewport.MinDepth = 0.0f;
//	m_ScreenViewport.MaxDepth = 1.0f;
//}
//
//
//void FDx11Renderer::Update()
//{
//
//}
//
//void FDx11Renderer::Draw()
//{
//
//}
//void FDx11Renderer::InitSinglePass()
//{
//	singlePass = new FDx11Pass(1, m_ScreenViewport, device );
//
//}
//
//void FDx11Renderer::ExecuteMainPass(FDx11Pass* pass)
//{
//
//	// Set Viewport
//	//device.GetDeviceContext()->RSSetViewports(1, &m_ScreenViewport);
//	//device.GetDeviceContext()->RSSetScissorRects(1, pass->GetScissorRects());
//
//	// Clear&Set FrameBuffer
//	ClearFrameBuffer(pass);
//	
//	//device.GetDeviceContext()->OMSetRenderTargets(1, pass->mMainRTV.GetAddressOf(), pass->GetDepthStencilView());
//	device.GetDeviceContext()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
//	// Render state
//	//SetRenderStates(gBufferPass->GetRenderStates());
//	SetRenderStates();
//	device.GetDeviceContext()->IASetInputLayout(pass->GetGpuProgram()->inputLayout.Get());
//	// shader & shader resource
//	SetGpuProgram(pass->GetGpuProgram());
//	// vertex inputlayout
//	//evice.GetDeviceContext()->IASetInputLayout(pass->GetInputLayout()->GetD3D11InputLayout());
//	
//	// Draw GameObject
//
//	//pImpl->m_CBFrame.SetVS(deviceContext, 0);
//	//pImpl->m_CBOnResize.SetVS(deviceContext, 1);
//	//pImpl->m_CBObjDrawing.SetPS(deviceContext, 2); //material
//	//pImpl->m_CBRarely.SetPS(deviceContext, 3);
//
//}
//void FDx11Renderer::ExecutePass(FDx11Pass* gBufferPass)
//{
//	
//	// Set Viewport
//	device.GetDeviceContext()->RSSetViewports(gBufferPass->GetNumViews(), gBufferPass->GetViewport());
//	device.GetDeviceContext()->RSSetScissorRects(1,gBufferPass->GetScissorRects());
//	
//	// Clear&Set FrameBuffer
//	ClearFrameBuffer(gBufferPass);
//	device.GetDeviceContext()->OMSetRenderTargets(gBufferPass->GetNumViews(), gBufferPass->GetRenderTargetViewAddress(), gBufferPass->GetDepthStencilView());
//	//device.GetDeviceContext()->OMSetRenderTargets(1, gBufferPass->mMainRTV.GetAddressOf(), gBufferPass->GetDepthStencilView());
//	// Render state
//	//SetRenderStates(gBufferPass->GetRenderStates());
//	SetRenderStates();
//	// shader & shader resource
//	SetGpuProgram(gBufferPass->GetGpuProgram());
//	// vertex inputlayout
//	device.GetDeviceContext()->IASetInputLayout(gBufferPass->GetInputLayout()->GetD3D11InputLayout());
//	// Draw GameObject
//
//}
//
//
//void FDx11Renderer::SetGpuProgram(FDx11GpuProgram* program)
//{
//	// shader
//	device.GetDeviceContext()->VSSetShader(program->GetVertexShader(), program->GetClassInstance(ShaderType::ST_Vertex),
//		program->GetNumInstance(ShaderType::ST_Vertex));
//	device.GetDeviceContext()->PSSetShader(program->GetPixelShader(), program->GetClassInstance(ShaderType::ST_Pixel),
//		program->GetNumInstance(ShaderType::ST_Pixel));
//
//	// 一个slot 只设置一块resource element ,不设置成Array magicNumber = 1
//	// shader resource constant buffer
//	unsigned int num = program->GetConstantBufferNum();
//	for (unsigned int i = 0; i < num; i++)
//	{
//		ID3D11Buffer* bufferArray[1];
//		ConstantBufferObject* cbo = program->GetContantBufferObject(0);
//		bufferArray[0] = cbo->GetBufferView();
//
//		UINT slot = i;
//		device.GetDeviceContext()->VSSetConstantBuffers(slot, 1, bufferArray);
//	}
//	ID3D11Buffer* bufferArray[1];
//
//	bufferArray[0] = ConstantBufferPool::Instance().GetResource("frame")->GetBufferView();
//	device.GetDeviceContext()->VSSetConstantBuffers(0, 1, bufferArray);
//	ID3D11Buffer* bufferArray2[1];
//	bufferArray2[0] = ConstantBufferPool::Instance().GetResource("onResize")->GetBufferView();
//	device.GetDeviceContext()->VSSetConstantBuffers(1, 1, bufferArray2);
//
//	// shader resource texture
//	num = program->GetTextureNum();
//	for (unsigned int i = 0; i < num; i++)
//	{
//
//		UINT slot = i;
//		UINT num = 1;
//		device.GetDeviceContext()->PSSetShaderResources(slot, num, program->GetGpuTextureView(i));
//	}
//
//	// shader resource sampler
//	num = program->GetSamplerNum();
//	for (unsigned int i = 0; i < num; i++)
//	{
//		UINT slot = i;
//		UINT num = 1;
//		device.GetDeviceContext()->PSSetSamplers(slot, num, program->GetSamplerView(i));
//	}
//
//}
//
//void FDx11Renderer::ClearFrameBuffer(FDx11Pass* GBufferPass)
//{
//	/*ID3D11RenderTargetView* pRTView[MAX_MULTIPLE_RENDER_TARGETS];
//	memset(pRTView, 0, sizeof(pRTView));*/
//
//	/*for (unsigned int i = 0; i < MAX_MULTIPLE_RENDER_TARGETS; i++)
//	{
//		pRTView[i] = GBufferPass->GetRenderTargetView(i);
//		if (!pRTView[i])
//		{
//			break;
//		}
//	}*/
//
//	float depth = 1.0;
//	UINT8 stencil = 0;
//	static float black[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,0,255)
//
//	unsigned int numberOfViews = GBufferPass->GetNumViews();
//	/*for (unsigned int i = 0; i < numberOfViews; ++i)
//	{
//		device.GetDeviceContext()->ClearRenderTargetView(GBufferPass->GetRenderTargetView(i), reinterpret_cast<const float*>(&black));
//	}*/
//	device.GetDeviceContext()->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&black));
//
//	UINT ClearFlags = 0;
//	ClearFlags |= D3D11_CLEAR_DEPTH;
//	ClearFlags |= D3D11_CLEAR_STENCIL;
//
//
//	device.GetDeviceContext()->ClearDepthStencilView(
//		GBufferPass->GetDepthStencilView(),
//		ClearFlags, depth, static_cast<UINT8>(stencil));
//	device.GetDeviceContext()->ClearDepthStencilView(
//		m_pDepthStencilView.Get(),
//		ClearFlags, depth, static_cast<UINT8>(stencil));
//}
//
//void FDx11Renderer::SetRenderStates()
//{
//	
//	device.GetDeviceContext()->RSSetState(RenderStates::RSCullClockWise.Get());              // rasterize 
//	device.GetDeviceContext()->OMSetDepthStencilState(RenderStates::DSSLessEqual.Get(), 0);  // depthStencil 
//	float factor[4] = { 0.5f ,0.5f, 0.5f, 0.5f };
//	device.GetDeviceContext()->OMSetBlendState(RenderStates::BSTransparent.Get(), factor, 0xFFFFFFFF);  // blend
//	/*device.GetDeviceContext()->OMSetDepthStencilState(nullptr, 0);
//	device.GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);*/
//}
//
//int FDx11Renderer::Run()
//{
//	MSG msg = { 0 };
//
//	mTimer.Reset();
//
//	while (msg.message != WM_QUIT)
//	{
//		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			mTimer.Tick();
//
//			if (!m_AppPaused)
//			{
//				//CalculateFrameStats();
//				//UpdateScene(m_Timer.DeltaTime());
//				Draw();
//			}
//			else
//			{
//				Sleep(100);
//			}
//		}
//	}
//
//	return (int)msg.wParam;
//}
//
//
//
//void FDx11Renderer::InitSamplerResourcePool()
//{
//	ComPtr<ID3D11SamplerState> samplerLinearWrap;
//
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//
//	// 线性过滤模式
//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	HR(device.GetDevice()->CreateSamplerState(&sampDesc, samplerLinearWrap.GetAddressOf()));
//
//	SamplerResoucePool::Instance().CreateResouce(SamplerType::SSLinearWrap, samplerLinearWrap);
//
//	ComPtr<ID3D11SamplerState> samplerAnistropicWrap;
//	
//	// 各向异性过滤模式
//	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MaxAnisotropy = 4;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	HR(device.GetDevice()->CreateSamplerState(&sampDesc, samplerAnistropicWrap.GetAddressOf()));
//	SamplerResoucePool::Instance().CreateResouce(SamplerType::SSAnistropicWrap, samplerAnistropicWrap);
//
//}
//
//void FDx11Renderer::OnResize()
//{
//	assert(device.GetDeviceContext());
//	assert(device.GetDevice());
//	assert(mSwapChain);
//	
//	
//	// 释放渲染管线输出用到的相关资源
//	m_pRenderTargetView.Reset();
//	m_pDepthStencilView.Reset();
//	m_pDepthStencilBuffer.Reset();
//
//	// 重设交换链并且重新创建渲染目标视图
//	ComPtr<ID3D11Texture2D> backBuffer;
//	HR(mSwapChain->ResizeBuffers(1, 800, 600, DXGI_FORMAT_B8G8R8A8_UNORM, 0));	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
//	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
//	HR(device.GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));
//
//	// 设置调试对象名
//	D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");
//	//string bb = "BackBuffer[0]";
//	//backBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)bb.length(), bb.c_str());
//
//	//backBuffer.Reset();
//
//
//	D3D11_TEXTURE2D_DESC depthStencilDesc;
//
//	depthStencilDesc.Width = 800;
//	depthStencilDesc.Height = 600;
//	depthStencilDesc.MipLevels = 1;
//	depthStencilDesc.ArraySize = 1;
//	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//	// 要使用 4X MSAA?
//	if (m_Enable4xMsaa)
//	{
//		depthStencilDesc.SampleDesc.Count = 4;
//		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
//	}
//	else
//	{
//		depthStencilDesc.SampleDesc.Count = 1;
//		depthStencilDesc.SampleDesc.Quality = 0;
//	}
//
//
//	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
//	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	depthStencilDesc.CPUAccessFlags = 0;
//	depthStencilDesc.MiscFlags = 0;
//
//	// 创建深度缓冲区以及深度模板视图
//	HR(device.GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));
//	HR(device.GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf()));
//
//
//	// 将渲染目标视图和深度/模板缓冲区结合到管线
//	device.GetDeviceContext()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
//
//	// 设置视口变换
//	m_ScreenViewport.TopLeftX = 0;
//	m_ScreenViewport.TopLeftY = 0;
//	m_ScreenViewport.Width = static_cast<float>(800);
//	m_ScreenViewport.Height = static_cast<float>(600);
//	m_ScreenViewport.MinDepth = 0.0f;
//	m_ScreenViewport.MaxDepth = 1.0f;
//
//	device.GetDeviceContext()->RSSetViewports(1, &m_ScreenViewport);
//
//}
//
//bool FDx11Renderer::InitDirect3D(FGraphicContext::Traits* traits, HWND hwnd)
//{
//	HRESULT hr = S_OK;
//
//	// 创建D3D设备 和 D3D设备上下文
//	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Direct2D需要支持BGRA格式
//#if defined(DEBUG) || defined(_DEBUG)  
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//	// 驱动类型数组
//	D3D_DRIVER_TYPE driverTypes[] =
//	{
//		D3D_DRIVER_TYPE_HARDWARE,
//		D3D_DRIVER_TYPE_HARDWARE,
//		D3D_DRIVER_TYPE_WARP,
//		D3D_DRIVER_TYPE_REFERENCE,
//	};
//	UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//	// 特性等级数组
//	D3D_FEATURE_LEVEL featureLevels[] =
//	{
//		D3D_FEATURE_LEVEL_11_0,
//	};
//	UINT numFeatureLevels = 1;
//	//ARRAYSIZE(featureLevels);
//
//	D3D_FEATURE_LEVEL featureLevel;
//	D3D_DRIVER_TYPE d3dDriverType;
//	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//	{
//		d3dDriverType = driverTypes[driverTypeIndex];
//		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
//			D3D11_SDK_VERSION, device.device.GetAddressOf(), &featureLevel, device.deviceContext.GetAddressOf());
//
//		if (hr == E_INVALIDARG)
//		{
//			// Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
//			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
//				D3D11_SDK_VERSION, device.device.GetAddressOf(), &featureLevel, device.deviceContext.GetAddressOf());
//		}
//
//		if (SUCCEEDED(hr))
//			break;
//	}
//
//	if (FAILED(hr))
//	{
//		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
//		return false;
//	}
//
//	// 检测是否支持特性等级11.0或11.1
//	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
//	{
//		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
//		return false;
//	}
//	
//	// 检测 MSAA支持的质量等级
//	device.GetDevice()->CheckMultisampleQualityLevels(
//		DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m_4xMsaaQuality);	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
//	assert(m_4xMsaaQuality > 0);
//
//	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
//	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
//	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(包含DXGI1.1)的接口类
//	//ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(包含DXGI1.2)特有的接口类
//
//
//	// 为了正确创建 DXGI交换链，首先我们需要获取创建 D3D设备 的 DXGI工厂，否则会引发报错：
//	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
//	//HR((device.GetDevice().As(&dxgiDevice));
//	HR(device.device.As(&dxgiDevice));
//
//	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
//	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));
//
//
//	// 填充DXGI_SWAP_CHAIN_DESC用以描述交换链
//	DXGI_SWAP_CHAIN_DESC sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.BufferDesc.Width = traits->width;
//	sd.BufferDesc.Height = traits->height;
//	sd.BufferDesc.RefreshRate.Numerator = 60;
//	sd.BufferDesc.RefreshRate.Denominator = 1;
//	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// 注意此处DXGI_FORMAT_B8G8R8A8_UNORM
//	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//	// 是否开启4倍多重采样？
//	if (m_Enable4xMsaa)
//	{
//		sd.SampleDesc.Count = 4;
//		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
//	}
//	else
//	{
//		sd.SampleDesc.Count = 1;
//		sd.SampleDesc.Quality = 0;
//	}
//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	sd.BufferCount = 1;
//	sd.OutputWindow = hwnd;
//	sd.Windowed = TRUE;
//	sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
//	sd.Flags = 0;
//	HR(dxgiFactory1->CreateSwapChain(device.GetDevice(), &sd, mSwapChain.GetAddressOf()));
//
//	OnResize();
//
//	return true;
//}
//
//
//
//
//
