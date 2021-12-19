
#include "UI/framework.h"
#include "Graphics/ACommonIncludes.h"

int main()
{
#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
	ALOG("Initializing ... !");
	return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}