#include"test.h"

#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include <crtdbg.h>

#include"FView.h"
#include<fstream>



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
	_In_ LPSTR cmdLine, _In_ int showCmd)
{
	// 这些参数不使用
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);

	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(179);

	/*FCore coreApp(hInstance);
	coreApp.Realize();*/

	FGraphicWindowWin32 window(hInstance);

	FDx11App application(window.GetTraits(), window.GetMainWnd());
	application.Initialize();
	application.Run();


	system("pause");
	return 0;
}




