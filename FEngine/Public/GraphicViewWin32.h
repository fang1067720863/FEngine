#pragma once

#include"View.h"

#include<Windows.h>
#include<windowsx.h>

class FGraphicViewWin32 : public FView
{

protected:
	void MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

#ifdef USE_IMGUI
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
#endif
		HandleNativeWin32Msg(msg, wParam, lParam);


	}


	void HandleNativeWin32Msg(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// WM_ACTIVATE is sent when the window is activated or deactivated.
			// We pause the game when the window is deactivated and unpause it
			// when it becomes active.
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)

				return;

			// WM_SIZE is sent when the user resizes the window.
		case WM_SIZE:
			// Save the new client area dimensions.
			//mRenderer->SetClientWidth(LOWORD(lParam));
			//mRenderer->SetClientHeight(HIWORD(lParam));
			//if (mRenderer->IsRunning())
			//{
			//	if (wParam == SIZE_MINIMIZED)
			//	{
			///*		mAppPaused = true;
			//		mMinimized = true;
			//		mMaximized = false;*/
			//	}
			//	else if (wParam == SIZE_MAXIMIZED)
			//	{
			//	
			//	}
			//	else if (wParam == SIZE_RESTORED)
			//	{






			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		case WM_ENTERSIZEMOVE:


			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:


			// WM_DESTROY is sent when the window is being destroyed.
		case WM_DESTROY:


			// The WM_MENUCHAR message is sent when a menu is active and the user presses
			// a key that does not correspond to any mnemonic or accelerator key.
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return; MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return; 0;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			// OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return; 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			// OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return; 0;
		case WM_MOUSEMOVE:
			float mx = GET_X_LPARAM(lParam);
			float my = GET_Y_LPARAM(lParam);
			// transformMouseXY(mx, my);
			GetEventQueue()->MouseMotion(mx, my);
			return;
		case WM_KEYUP:
			if (wParam == VK_ESCAPE)
			{
				;//PostQuitMessage(0);
			}

			return; 0;
		}

	}
};