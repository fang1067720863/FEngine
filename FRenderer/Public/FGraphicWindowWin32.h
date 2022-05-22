#pragma once
#include"GraphicContext.h"
#include<Windows.h>

class FGraphicWindowWin32: public FGraphicWindow
{
public:
	FGraphicWindowWin32(){}
	FGraphicWindowWin32(HINSTANCE hInstance)
		: m_hAppInst(hInstance){}
	FGraphicWindowWin32(FGraphicContext::Traits* traits =nullptr);
	FGraphicWindowWin32(int x, int y, int width, int height);
	~FGraphicWindowWin32();

	bool CreateMainWindow();
	void SetAppInstance(HINSTANCE hInstance)
	{
		m_hAppInst = hInstance;
	}
	HWND GetMainWnd() {
		return m_hMainWnd;
	}

	
protected:
	bool Init() override;
	bool SetWindowPosAndStyle();
	bool InitMainWindow();
	
	void TransformMouseXY(float& x, float& y);
	bool HandleNativeWin32Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HINSTANCE m_hAppInst;        // 应用实例句柄
	HWND      m_hMainWnd;        // 主窗口句柄

};
