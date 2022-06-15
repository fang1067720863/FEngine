#pragma once

#include<string>
#include"Ptr.h"

class FGraphicContext
{
public:
    FGraphicContext()
    {
        _traits = new Traits();
    }

    virtual ~FGraphicContext()
    {
        delete _traits;
    }

    struct Traits 
    {
    public:
        // graphics context original and size
        int x = 0;
        int y = 0;
        int width = 800;
        int height = 600;

        std::string windowingSystemPreference;

        // window decoration and behaviour
        std::string windowName;
        bool        windowDecoration;
        bool        supportsResize;

        //HINSTANCE m_hAppInst;        // Ӧ��ʵ�����
        //HWND      m_hMainWnd;        // �����ھ��
        //bool      m_AppPaused;       // Ӧ���Ƿ���ͣ
        //bool      m_Minimized;       // Ӧ���Ƿ���С��
        //bool      m_Maximized;       // Ӧ���Ƿ����
        //bool      m_Resizing;        // ���ڴ�С�Ƿ�仯
        //bool	  m_Enable4xMsaa;	 // �Ƿ���4�����ز���
        //UINT      m_4xMsaaQuality;   // MSAA֧�ֵ������ȼ�


        // multi sample parameters
        unsigned int sampleBuffers;
        unsigned int samples;

        // V-sync
        bool            vsync = true;

        // Swap Group

        // use multithreaded OpenGL-engine (OS X only)

        // enable cursor
        bool            useCursor;

   /*     std::string     glContextVersion;
        unsigned int    glContextFlags;
        unsigned int    glContextProfileMask;*/
        // ask the GraphicsWindow implementation to set the pixel format of an inherited window
        bool setInheritedWindowPixelFormat;

        // X11 hint whether to override the window managers window size/position redirection
        bool overrideRedirect;

    };

    Traits* _traits;

    Traits* GetTraits() {
        return _traits;
    }


};

class FGraphicWindow: public FGraphicContext
{
public:
    FGraphicWindow():FGraphicContext(){}
    ~FGraphicWindow() {};

    virtual bool IsInit() { return _initialized; }

    virtual bool Init() { return false; }

protected:
    virtual bool InitImplementation() { return false; }
    virtual bool Release()         { return false; }
    virtual bool SwapBuffers() { return false; }
    virtual bool WindowRect() { return false; }
    virtual bool RaiseWindow() { return false; }
   


    int             _screenOriginX;
    int             _screenOriginY;
    unsigned int    _screenWidth;
    unsigned int    _screenHeight;

    int             _windowOriginXToRealize;
    int             _windowOriginYToRealize;
    unsigned int    _windowWidthToRealize;
    unsigned int    _windowHeightToRealize;

    bool            _initialized;
    bool            _valid;
    bool            _realized;

    bool            _ownsWindow;
    bool            _closeWindow;
    bool            _destroyWindow;
    bool            _destroying;

    //FPtr<FEventQueue>    _eventQueue;


};

