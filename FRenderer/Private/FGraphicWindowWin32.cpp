#include"FGraphicWindowWin32.h"
#include<windowsx.h>

FGraphicWindowWin32::FGraphicWindowWin32(FGraphicContext::Traits* traits)
{

}

FGraphicWindowWin32::FGraphicWindowWin32(int x, int y, int width, int height)
{
    _traits = new Traits;
    _traits->x = x;
    _traits->y = y;
    _traits->width = width;
    _traits->height = height;

    //CreateMainWindow();
}

FGraphicWindowWin32::~FGraphicWindowWin32()
{

}
bool FGraphicWindowWin32::Init()
{
    return false;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC         hdc;  //设备环境句柄
	PAINTSTRUCT ps;
	RECT        rect;
	switch (message) {
		//窗口绘制消息
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(
			hdc,
			TEXT("你好，欢迎来到C语言中文网"),
			-1,
			&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER
		);
		EndPaint(hwnd, &ps);
		return 0;
		//窗口销毁消息
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool FGraphicWindowWin32::CreateMainWindow()
{
	if (!SetWindowPosAndStyle())
	{

	}
	

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);


	m_hMainWnd = CreateWindow(L"D3DWndClassName", L"234",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, m_hAppInst, 0);

	if (!m_hMainWnd)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, L"", MB_OK);
		LocalFree(lpMsgBuf);
		// MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);

	return true;
}
bool FGraphicWindowWin32::SetWindowPosAndStyle()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}
	// 将窗口调整到中心
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Compute window rectangle dimensions based on requested client area dimensions.
	/*RECT R = { 0, 0, 800, 600 };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;*/
	return true;
}

bool FGraphicWindowWin32::InitMainWindow()
{
	return true;

}

void FGraphicWindowWin32::TransformMouseXY(float& x, float& y)
{
   /* x = eventState->getXmin() + (eventState->getXmax() - eventState->getXmin()) * x / float(_traits->width);
    y = eventState->getYmin() + (eventState->getYmax() - eventState->getYmin()) * y / float(_traits->height);*/
}

bool FGraphicWindowWin32::HandleNativeWin32Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    //double eventTime = getEventQueue()->getTime();
    //_timeOfLastCheckEvents = eventTime;


    switch (uMsg)
    {
        // Wojtek Lewandowski 2010-09-28:
        // All web docs on Windows Aero and OpenGL compatibiltiy
        // suggest WM_ERASEBKGND should be handled with non NULL value return.
        // This sugesstion may be irrelevant for our window class
        // as default brush pattern is not set so erase flag is forwarded to WM_PAINT
        // and gets ignored when WM_PAINT is handled.
        // But it will certainly be safer and not make things worse
        // if we handle this message to be sure everything is done as suggested.
    case WM_ERASEBKGND:
        return TRUE;
        break;

        /////////////////
    case WM_PAINT:
        /////////////////
        PAINTSTRUCT paint;
        ::BeginPaint(hwnd, &paint);
        ::EndPaint(hwnd, &paint);
       /* requestRedraw();*/
      /*  if (_ownsWindow)
        {
            
        }*/
        break;

        ///////////////////
    case WM_MOUSEMOVE:
        ///////////////////

    {
        float mx = GET_X_LPARAM(lParam);
        float my = GET_Y_LPARAM(lParam);
        TransformMouseXY(mx, my);
       // getEventQueue()->MouseMotion(mx, my);
    }
    break;

    /////////////////////
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        /////////////////////

    {
       /* ::SetCapture(hwnd);

        int button ;

        if (uMsg == WM_LBUTTONDOWN)      button = 1;
        else if (uMsg == WM_MBUTTONDOWN) button = 2;
        else button = 3;

        _capturedMouseButtons.insert(button);

        float mx = GET_X_LPARAM(lParam);
        float my = GET_Y_LPARAM(lParam);
        TransformMouseXY(mx, my);
        getEventQueue()->mouseButtonPress(mx, my, button, eventTime);*/
    }
    break;

    /////////////////////
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        /////////////////////

    {
        int button;

        if (uMsg == WM_LBUTTONUP)      button = 1;
        else if (uMsg == WM_MBUTTONUP) button = 2;
        else button = 3;

        /*_capturedMouseButtons.erase(button);

        if (_capturedMouseButtons.empty())
            ::ReleaseCapture();*/

        float mx = GET_X_LPARAM(lParam);
        float my = GET_Y_LPARAM(lParam);
        TransformMouseXY(mx, my);
        //getEventQueue()->MouseButtonRelease(mx, my, button);
    }
    break;

    ///////////////////////
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        ///////////////////////

    {
        ::SetCapture(hwnd);

      //  int button;

      //  if (uMsg == WM_LBUTTONDBLCLK)            button = 1;
      //  else if (uMsg == WM_MBUTTONDBLCLK)    button = 2;
      //  else button = 3;

      ///*  _capturedMouseButtons.insert(button);*/

      //  float mx = GET_X_LPARAM(lParam);
      //  float my = GET_Y_LPARAM(lParam);
      //  TransformMouseXY(mx, my);
      //  getEventQueue()->MouseDoubleButtonPress(mx, my, button);
    }
    break;

    ////////////////////
    case WM_MOUSEWHEEL:
        ////////////////////

       /* getEventQueue()->MouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? FGUIEventQueue::ScrollMotion::Scroll_UP :
            FGUIEventQueue::ScrollMotion::Scroll_UP
            );*/
        break;

        /////////////////
    case WM_MOVE:
    case WM_SIZE:
        /////////////////

    {
        //int windowX = _traits->x, windowY = _traits->y, windowWidth = _traits->width, windowHeight = _traits->height;
        //if (areWindowDimensionsChanged(hwnd, _screenOriginX, _screenOriginY, windowX, windowY, windowWidth, windowHeight))
        //{
        //    resized(windowX, windowY, windowWidth, windowHeight);
        //    getEventQueue()->windowResize(windowX, windowY, windowWidth, windowHeight, eventTime);

        //    // request redraw if window size was changed
        //    if (windowWidth != _traits->width || windowHeight != _traits->height)
        //        requestRedraw();
        //}
    }
    break;

    ////////////////////
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        ////////////////////

    {
        int keySymbol = 0;
        int unmodifiedKeySymbol = 0;
        unsigned int modifierMask = 0;
        //adaptKey(wParam, lParam, keySymbol, modifierMask, unmodifiedKeySymbol);
        //_keyMap[std::make_pair(keySymbol, unmodifiedKeySymbol)] = true;
        ////getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
        //getEventQueue()->keyPress(keySymbol, eventTime, unmodifiedKeySymbol);
    }
    break;

    //////////////////
    case WM_KEYUP:
    case WM_SYSKEYUP:
        //////////////////

    {
        int keySymbol = 0;
        int unmodifiedKeySymbol = 0;
        unsigned int modifierMask = 0;
        //adaptKey(wParam, lParam, keySymbol, modifierMask, unmodifiedKeySymbol);
        //_keyMap[std::make_pair(keySymbol, unmodifiedKeySymbol)] = false;
        ////getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
        //getEventQueue()->keyRelease(keySymbol, eventTime, unmodifiedKeySymbol);
    }
    break;

    ///////////////////
    case WM_SETCURSOR:
        ///////////////////
            //The cursor is only modified in response to the WM_SETCURSOR message if the mouse cursor isn't set to
            //InheritCursor.  InheritCursor lets the user manage the cursor externally.
        /*if (_mouseCursor != InheritCursor)
        {
            if (_traits->useCursor)
                ::SetCursor(_currentCursor);
            else
                ::SetCursor(NULL);
            return TRUE;
        }
        break;*/

        ///////////////////
    case WM_SETFOCUS:
        ///////////////////
            // Check keys and send a message if the key is pressed when the
            // focus comes back to the window.
            // I don't really like this hard-coded loop, but the key codes
            // (VK_* constants) seem to go from 0x08 to 0xFE so it should be
            // ok. See winuser.h for the key codes.
        for (unsigned int i = 0x08; i < 0xFF; i++)
        {
            // Wojciech Lewandowski: 2011/09/12
            // Skip CONTROL | MENU | SHIFT tests because we are polling exact left or right keys
            // above return press for both right and left so we may end up with inconsistent
            // modifier mask if we report left control & right control while only right was pressed
            LONG rightSideCode = 0;
            switch (i)
            {
            case VK_CONTROL:
            case VK_SHIFT:
            case VK_MENU:
                continue;

            case VK_RCONTROL:
            case VK_RSHIFT:
            case VK_RMENU:
                rightSideCode = 0x01000000;
            }
            if ((::GetAsyncKeyState(i) & 0x8000) != 0)
            {
                // Compute lParam because subsequent adaptKey will rely on correct lParam
                UINT scanCode = ::MapVirtualKeyEx(i, 0, ::GetKeyboardLayout(0));
                // Set Extended Key bit + Scan Code + 30 bit to indicate key was set before sending message
                // See Windows SDK help on WM_KEYDOWN for explanation
                LONG lParamKey = rightSideCode | ((scanCode & 0xFF) << 16) | (1 << 30);
                ::SendMessage(hwnd, WM_KEYDOWN, i, lParamKey);
            }
        }
        break;

        ///////////////////
    case WM_KILLFOCUS:
        ///////////////////

            // Release all keys that were pressed when the window lost focus.
       /* for (std::map<std::pair<int, int>, bool>::iterator key = _keyMap.begin();
            key != _keyMap.end(); ++key)
        {
            if (key->second)
            {
                getEventQueue()->keyRelease(key->first.first, key->first.second);
                key->second = false;
            }
        }

        _capturedMouseButtons.clear();*/

        break;

 
    /////////////////
    case WM_CLOSE:
        /////////////////

        /*getEventQueue()->closeWindow(eventTime);*/
        break;

        /////////////////
    case WM_DESTROY:
        /////////////////

        _destroyWindow = true;
        if (_ownsWindow)
        {
            ::PostQuitMessage(0);
        }
        break;

        //////////////
    case WM_QUIT:
        //////////////

        _closeWindow = true;
        return wParam;

        //////////////
    case WM_TOUCH:
        /////////////
    {
      
    }
    break;

    case WM_POINTERDOWN:
        /////
    {
       
    }

    break;

    /////
    case WM_POINTERUPDATE:
        /////
    {
       
    }

    break;


    /////
    case WM_POINTERUP:
        /////
    {
      
    }

    break;


    /////////////////
    default:
        /////////////////

        if (_ownsWindow) return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
        break;
    }

    if (_ownsWindow) return 0;

    return 0;
        /*_windowProcedure == 0 ? ::DefWindowProc(hwnd, uMsg, wParam, lParam) :
        ::CallWindowProc(_windowProcedure, hwnd, uMsg, wParam, lParam);*/
}

bool TransformMouseXY(int& mx, int& my)
{
    return false;
}
