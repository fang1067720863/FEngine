#pragma once

#include<Windows.h>

class  FGraphicsHandlerWin32
{
public:

	FGraphicsHandlerWin32() :
		_hwnd(0),
		_hdc(0),
		_hglrc(0) {}

	/** Set native window.*/
	inline void setHWND(HWND hwnd) { _hwnd = hwnd; }

	/** Get native window.*/
	inline HWND getHWND() const { return _hwnd; }

	/** Set device context.*/
	inline void setHDC(HDC hdc) { _hdc = hdc; }

	/** Get device context.*/
	inline HDC getHDC() const { return _hdc; }

	/** Set native OpenGL graphics context.*/
	inline void setWGLContext(HGLRC hglrc) { _hglrc = hglrc; }

	/** Get native OpenGL graphics context.*/
	inline HGLRC getWGLContext() const { return _hglrc; }

protected:

	HWND            _hwnd;
	HDC             _hdc;
	HGLRC           _hglrc;

};
#endif
}