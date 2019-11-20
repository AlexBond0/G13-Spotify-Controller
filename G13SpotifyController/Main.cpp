#include <windows.h>

#include <LogitechLCDLib.h>
#include "Requester.h"
#include "Window.h"

#include <string>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	// program starts here

	OutputDebugString("Working...");

	std::wstring message = L"Spotify Controller";

	// connect to G13
	bool ret = LogiLcdInit(&message[0], LOGI_LCD_TYPE_MONO);
	OutputDebugString((ret == TRUE) ? "TRUE" : "FALSE");

	/*LogiLcdUpdate();

	std::wstring pingus = L"This will display my";
	LogiLcdMonoSetText(0, &pingus[0]);

	pingus = L"spotify info";
	LogiLcdMonoSetText(1, &pingus[0]);

	LogiLcdUpdate();*/

	// Requester::TestRequest();

	Window w = Window();

	LogiLcdShutdown();
	return 0;
}
