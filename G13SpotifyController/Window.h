#pragma once

#include <Windows.h>
#include <string>
#include <LogitechLCDLib.h>
#include <vector>

#include "Tools.h"
#include "G13.h"


#define RENDER_TIMER 1001
#define INPUT_TIMER 1002

class Window
{
public:
	Window();
	~Window();

	void Run();

protected:

	bool BTN0;
	bool BTN1;
	bool BTN2;
	bool BTN3;

	std::vector<Component*> components;

private:

	int renderTimer;
	int inputTimer;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CheckInput();
	void Render();
};

