#include "d3dApp.h"

#include"FDx11.h"
#include <windowsx.h>
#include <sstream>
#include"FDx11RenderState.h"
#include"FDx11Pipeline.h"

#pragma warning(disable: 6031)
#include"Event.h"
extern "C"
{
    // 在具有多显卡的硬件设备中，优先使用NVIDIA或AMD的显卡运行
    // 需要在.exe中使用
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
}

namespace
{
    // This is just used to forward Windows messages from a global window
    // procedure to our member function window procedure because we cannot
    // assign a member function to WNDCLASS::lpfnWndProc.
    D3DApp* g_pd3dApp = nullptr;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
    // before CreateWindow returns, and thus before m_hMainWnd is valid.
    return g_pd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : m_hAppInst(hInstance),
    m_MainWndCaption(windowName),
    m_ClientWidth(initWidth),
    m_ClientHeight(initHeight),
    m_hMainWnd(nullptr),
    m_AppPaused(false),
    m_Minimized(false),
    m_Maximized(false),
    m_Resizing(false),
    m_Enable4xMsaa(true),
    m_4xMsaaQuality(0),
    //m_pd3dDevice(nullptr),
    //m_pd3dImmediateContext(nullptr),
    m_pSwapChain(nullptr),
    m_pDepthStencilBuffer(nullptr),
    m_pRenderTargetView(nullptr),
    m_pDepthStencilView(nullptr)
{
    ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));


    // 让一个全局指针获取这个类，这样我们就可以在Windows消息处理的回调函数
    // 让这个类调用内部的回调函数了
    g_pd3dApp = this;

    PassBuilder::Instance().SetDevice(&m_pDevice);
    PassBuilder::Instance().context->mainScreenViewport = m_ScreenViewport;
}

D3DApp::~D3DApp()
{
    // 恢复所有默认设定
    if (m_pDevice.deviceContext)
        m_pDevice.deviceContext->ClearState();
        //m_pd3dImmm_pDevice.deviceContextediateContext->ClearState();
}

HINSTANCE D3DApp::AppInst()const
{
    return m_hAppInst;
}

HWND D3DApp::MainWnd()const
{
    return m_hMainWnd;
}

float D3DApp::AspectRatio()const
{
    return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}

int D3DApp::Run()
{
    MSG msg = { 0 };

    //m_Timer.Reset();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //m_Timer.Tick();

            if (!m_AppPaused)
            {
                CalculateFrameStats();
                UpdateScene(0.1f);
                DrawScene();
            }
            else
            {
                Sleep(100);
            }
        }
    }

    return (int)msg.wParam;
}

bool D3DApp::Init()
{
    if (!InitMainWindow())
        return false;

    if (!InitDirect3D())
        return false;
    SamplerResoucePool::Instance().Init(m_pDevice);
    RasterStateResoucePool::Instance().Init(m_pDevice);
    BlendStateResoucePool::Instance().Init(m_pDevice);
    DepthStencilStateResoucePool::Instance().Init(m_pDevice);
   
  

    return true;
}

bool D3DApp::PostInit()
{
    pipeline = new FDx11Pipeline(m_pDevice);
    pipeline->Init();
    return false;
}

void D3DApp::OnResize()
{
    assert(m_pDevice.deviceContext);
    assert(m_pDevice.device);
    assert(m_pSwapChain);



    // 释放渲染管线输出用到的相关资源
    m_pRenderTargetView.Reset();
    m_pDepthStencilView.Reset();
    m_pDepthStencilBuffer.Reset();

    // 重设交换链并且重新创建渲染目标视图
    ComPtr<ID3D11Texture2D> backBuffer;
    HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
    HR(m_pDevice.device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));

   

    // 设置调试对象名
    D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");

    backBuffer.Reset();


    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = m_ClientWidth;
    depthStencilDesc.Height = m_ClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // 要使用 4X MSAA?
    if (m_Enable4xMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }



    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // 创建深度缓冲区以及深度模板视图
    HR(m_pDevice.device->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));
    HR(m_pDevice.device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf()));


    // 将渲染目标视图和深度/模板缓冲区结合到管线
    m_pDevice.deviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    


    // 设置视口变换
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
    m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_pDevice.deviceContext->RSSetViewports(1, &m_ScreenViewport);

    PassBuilder::Instance().context->mainDepthStencilView = m_pDepthStencilView;
    PassBuilder::Instance().context->mainRenderTargetView = m_pRenderTargetView;
    PassBuilder::Instance().context->mainScreenViewport = m_ScreenViewport;

    // 设置调试对象名
    D3D11SetDebugObjectName(m_pDepthStencilBuffer.Get(), "DepthStencilBuffer");
    D3D11SetDebugObjectName(m_pDepthStencilView.Get(), "DepthStencilView");
    D3D11SetDebugObjectName(m_pRenderTargetView.Get(), "BackBufferRTV[0]");

}

void D3DApp::ResetMainRenderTarget()
{
    m_pDevice.deviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    // 设置视口变换
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(m_ClientWidth);
    m_ScreenViewport.Height = static_cast<float>(m_ClientHeight);
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_pDevice.deviceContext->RSSetViewports(1, &m_ScreenViewport);
}

LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


    switch (msg)
    {
        // WM_ACTIVATE is sent when the window is activated or deactivated.  
        // We pause the game when the window is deactivated and unpause it 
        // when it becomes active.  
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
        {
            m_AppPaused = true;
            //m_Timer.Stop();
        }
        else
        {
            m_AppPaused = false;
            //m_Timer.Start();
        }
        return 0;

        // WM_SIZE is sent when the user resizes the window.  
    case WM_SIZE:
        // Save the new client area dimensions.
        m_ClientWidth = LOWORD(lParam);
        m_ClientHeight = HIWORD(lParam);
        if (m_pDevice.device)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                m_AppPaused = true;
                m_Minimized = true;
                m_Maximized = false;
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                m_AppPaused = false;
                m_Minimized = false;
                m_Maximized = true;
                OnResize();
            }
            else if (wParam == SIZE_RESTORED)
            {

                // Restoring from minimized state?
                if (m_Minimized)
                {
                    m_AppPaused = false;
                    m_Minimized = false;
                    OnResize();
                }

                // Restoring from maximized state?
                else if (m_Maximized)
                {
                    m_AppPaused = false;
                    m_Maximized = false;
                    OnResize();
                }
                else if (m_Resizing)
                {
                    // If user is dragging the resize bars, we do not resize 
                    // the buffers here because as the user continuously 
                    // drags the resize bars, a stream of WM_SIZE messages are
                    // sent to the window, and it would be pointless (and slow)
                    // to resize for each WM_SIZE message received from dragging
                    // the resize bars.  So instead, we reset after the user is 
                    // done resizing the window and releases the resize bars, which 
                    // sends a WM_EXITSIZEMOVE message.
                }
                else // API call such as SetWindowPos or m_pSwapChain->SetFullscreenState.
                {
                    OnResize();
                }
            }
        }
        return 0;

        // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
    case WM_ENTERSIZEMOVE:
        m_AppPaused = true;
        m_Resizing = true;
        //m_Timer.Stop();
        return 0;

        // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
    case WM_EXITSIZEMOVE:
        m_AppPaused = false;
        m_Resizing = false;
        //m_Timer.Start();
       //OnResize();
        return 0;

        // WM_DESTROY is sent when the window is being destroyed.
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

        // The WM_MENUCHAR message is sent when a menu is active and the user presses 
        // a key that does not correspond to any mnemonic or accelerator key. 
    case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);

        // Catch this message so to prevent the window from becoming too small.
    case WM_GETMINMAXINFO:
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
        return 0;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    {
        float delta = Timer::Instance().DeltaTime();
        int32_t mx = GET_X_LPARAM(lParam);
        int32_t my = GET_Y_LPARAM(lParam);
        int button;
        eventQueue.TransformMouseXY(mx, my);
        if (msg == WM_LBUTTONDOWN)      button = 0;
        else if (msg == WM_MBUTTONDOWN) button = 1;
        else button = 2;
        eventQueue.SetDragBtn(button);
        eventQueue.MouseDrag(mx, my, delta,button);
        break;
    }
       
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {eventQueue.CancelDrag(); }
       
    case WM_MOUSEMOVE:
    {
        if (eventQueue.isDragged())
        {
            float delta = Timer::Instance().DeltaTime();
            int32_t mx = GET_X_LPARAM(lParam);
            int32_t my = GET_Y_LPARAM(lParam);

            eventQueue.TransformMouseXY(mx, my);
            eventQueue.MouseDrag(mx, my, delta, eventQueue.GetDragBtn());
        }
        break;
       

    }
    case WM_MOUSEWHEEL:
    {
        ////////////////////
        float delta = Timer::Instance().DeltaTime();
        eventQueue.MouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? ScrollingMotion::SCROLL_DOWN : ScrollingMotion::SCROLL_UP, delta);
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        KeySymbol keySymbol, modifiedKeySymbol;
        KeyModifier keyModifier = KeyModifier::MODKEY_Alt;

        eventQueue.KeyUp(wParam, lParam, keySymbol, modifiedKeySymbol, keyModifier);
        break;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool D3DApp::InitMainWindow()
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hAppInst;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = L"D3DWndClassName";

    if (!RegisterClass(&wc))
    {
        MessageBox(0, L"RegisterClass Failed.", 0, 0);
        return false;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    m_hMainWnd = CreateWindow(L"D3DWndClassName", m_MainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
    if (!m_hMainWnd)
    {
        MessageBox(0, L"CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(m_hMainWnd, SW_SHOW);
    UpdateWindow(m_hMainWnd);

    return true;
}

bool D3DApp::InitDirect3D()
{
    HRESULT hr = S_OK;

    // 创建D3D设备 和 D3D设备上下文
    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // 驱动类型数组
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    // 特性等级数组
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_FEATURE_LEVEL featureLevel;
    D3D_DRIVER_TYPE d3dDriverType;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        d3dDriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, m_pDevice.device.GetAddressOf(), &featureLevel, m_pDevice.deviceContext.GetAddressOf());

        if (hr == E_INVALIDARG)
        {
            // Direct3D 11.0 的API不承认D3D_FEATURE_LEVEL_11_1，所以我们需要尝试特性等级11.0以及以下的版本
            hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, m_pDevice.device.GetAddressOf(), &featureLevel, m_pDevice.deviceContext.GetAddressOf());
        }

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
    {
        MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
        return false;
    }

    // 检测是否支持特性等级11.0或11.1
    if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }

    // 检测 MSAA支持的质量等级
    m_pDevice.device->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
    assert(m_4xMsaaQuality > 0);




    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
    ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(包含DXGI1.1)的接口类
    

    // 为了正确创建 DXGI交换链，首先我们需要获取创建 D3D设备 的 DXGI工厂，否则会引发报错：
    // "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
    HR(m_pDevice.device.As(&dxgiDevice));
    HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
    HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));


    // 填充DXGI_SWAP_CHAIN_DESC用以描述交换链
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = m_ClientWidth;
    sd.BufferDesc.Height = m_ClientHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    // 是否开启4倍多重采样？
    if (m_Enable4xMsaa)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = m_hMainWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    HR(dxgiFactory1->CreateSwapChain(m_pDevice.device.Get(), &sd, m_pSwapChain.GetAddressOf()));
    



    // 可以禁止alt+enter全屏
    dxgiFactory1->MakeWindowAssociation(m_hMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

    // 设置调试对象名


    // 每当窗口被重新调整大小的时候，都需要调用这个OnResize函数。现在调用
    // 以避免代码重复
    OnResize();

    return true;
}

void D3DApp::CalculateFrameStats()
{
    // 该代码计算每秒帧速，并计算每一帧渲染需要的时间，显示在窗口标题
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;

    //if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
    //{
    //    float fps = (float)frameCnt; // fps = frameCnt / 1
    //    float mspf = 1000.0f / fps;

    //    std::wostringstream outs;
    //    outs.precision(6);
    //    outs << m_MainWndCaption << L"    "
    //        << L"FPS: " << fps << L"    "
    //        << L"Frame Time: " << mspf << L" (ms)";
    //    SetWindowText(m_hMainWnd, outs.str().c_str());

    //    // Reset for next average.
    //    frameCnt = 0;
    //    timeElapsed += 1.0f;
    //}
}

