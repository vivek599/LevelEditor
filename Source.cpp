
#include "UI/framework.h"
#include "Graphics/ACommonIncludes.h"

int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
	ALOG(TEXT("Initializing ... !"));
	return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}