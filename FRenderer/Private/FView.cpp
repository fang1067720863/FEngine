#include "..\Public\FView.h"
#pragma once

FCore::FCore(HINSTANCE hInstance) 
	: m_hAppInst(hInstance) {}


bool FCore::Realize()
{
	window = CreateEmbbedWindow(0, 0, 800, 600);
	//renderer = new FDx11Renderer(window->GetTraits(),window->GetMainWnd());
	return true;

}

int FCore::Run()
{
	MSG msg = { 0 };

	m_Timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_Timer.Tick();

			if (!m_AppPaused)
			{
				//CalculateFrameStats();
				Update(m_Timer.DeltaTime());
				Draw();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

FGraphicWindowWin32* FCore::CreateEmbbedWindow(int x, int y, int width, int height)
{
	
	FGraphicWindowWin32* gw = new FGraphicWindowWin32(x, y, width, height);
	gw->SetAppInstance(m_hAppInst);
	gw->CreateMainWindow();
	return gw;
}

void FCore::Update(float dt)
{
	// guievent
	// camera
	// scene
}

void FCore::DrawSceneObjects(FRhiSceneObject* sObject)
{
	
}

void FCore::Draw()
{
	//assert(m_pd3dImmediateContext);
	//assert(m_pSwapChain);

	// Clear RenderTarget&DepthStencil 
	// SetRenderState
	// Set Shader
	// Resource bind
	// DrawCommand
	// SwapBuffer
	
	//IASetInputLayout(pImpl->m_pPosNormalTexLayout.Get());
	//VSSetShader
	//PSSetShader
	//deviceContext->IASetPrimitiveTopology
	//deviceContext->GSSetShader(nullptr
	//deviceContext->RSSetState(nullptr
	//deviceContext->PSSetSamplers
	//deviceContext->OMSetDepthStencilState
	//deviceContext->OMSetBlendState
	//D3D11_BUFFER_DESC bd;
	//device->CreateBuffer(&bd, &InitData, m_vbo.GetAddressOf());
	// HR(D3DX11CreateShaderResourceViewFromFile(device, string2wstring(m_diffusePath).c_str(), NULL, NULL, &m_texDiffuse, NULL));
	//deviceContext->VSSetConstantBuffers(slot, 1, bufferArray);
	//deviceContext->DrawIndexed(m_pIndexBuffer->GetNum(), 0, 0);


}
