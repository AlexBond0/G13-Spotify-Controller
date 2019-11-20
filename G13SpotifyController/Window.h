#pragma once

#include <Windows.h>
#include <string>
#include <LogitechLCDLib.h>

#include "Tools.h"

#define RENDER_TIMER 1001
#define INPUT_TIMER 1002

class Window
{
public:
	Window();
	~Window();

private:
	void CheckInput();
	void Render();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool BTN0;
	bool BTN1;
	bool BTN2;
	bool BTN3;

	int testNum;
};

