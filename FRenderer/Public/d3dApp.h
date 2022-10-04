#pragma once

#include <wrl/client.h>
#include <string>
#include <DirectXMath.h>
#include"FDx11.h"
#include<list>
#include"Event.h"
#include"EventQueue.h"
#include"Timer.h"
#include"FDx11ResourceFactory.h"

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();

    HINSTANCE AppInst()const;                   // ��ȡӦ��ʵ���ľ��
    HWND      MainWnd()const;                   // ��ȡ�����ھ��
    float     AspectRatio()const;               // ��ȡ��Ļ��߱�
    int aa = 1;
    int Run();                                  // ���г��򣬽�����Ϸ��ѭ��

    // ��ܷ������ͻ���������Ҫ������Щ������ʵ���ض���Ӧ������
    virtual bool Init();                        // �ø��෽����Ҫ��ʼ�����ں�Direct3D����
    virtual void OnResize();                    // �ø��෽����Ҫ�ڴ��ڴ�С�䶯��ʱ�����
    virtual void UpdateScene(float dt) = 0;     // ������Ҫʵ�ָ÷��������ÿһ֡�ĸ���
    virtual void DrawScene() = 0;               // ������Ҫʵ�ָ÷��������ÿһ֡�Ļ���
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    // ���ڵ���Ϣ�ص�����
protected:
    bool InitMainWindow();       // ���ڳ�ʼ��
    bool InitDirect3D();         // Direct3D��ʼ��

    void CalculateFrameStats();  // ����ÿ��֡�����ڴ�����ʾ
    void ResetMainRenderTarget();
protected:

    HINSTANCE m_hAppInst;        // Ӧ��ʵ�����
    HWND      m_hMainWnd;        // �����ھ��
    bool      m_AppPaused;       // Ӧ���Ƿ���ͣ
    bool      m_Minimized;       // Ӧ���Ƿ���С��
    bool      m_Maximized;       // Ӧ���Ƿ����
    bool      m_Resizing;        // ���ڴ�С�Ƿ�仯
    bool      m_Enable4xMsaa;    // �Ƿ���4�����ز���
    UINT      m_4xMsaaQuality;   // MSAA֧�ֵ������ȼ�


    Timer m_Timer;            // ��ʱ��

    // ʹ��ģ�����(C++11)��������
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    // Direct3D 11
    //ComPtr<ID3D11Device> m_pd3dDevice;                    // D3D11�豸
    //ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;   // D3D11�豸������
    FDx11Device m_pDevice;
    ComPtr<IDXGISwapChain> m_pSwapChain;                  // D3D11������
    // Direct3D 11.1

    //// ������Դ
    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;        // ���ģ�建����
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;   // ��ȾĿ����ͼ
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;   // ���ģ����ͼ
    D3D11_VIEWPORT m_ScreenViewport;                      // �ӿ�

    // ������Ӧ���ڹ��캯�����ú���Щ�Զ���ĳ�ʼ����
    std::wstring m_MainWndCaption;                        // �����ڱ���
    int m_ClientWidth;                                    // �ӿڿ��
    int m_ClientHeight;                                   // �ӿڸ߶�

    BufferedEventQueue eventQueue;

};


