#pragma once

#include <Windows.h>
#include <string>
#include <LogitechLCDLib.h>
#include <vector>

#include "Tools.h"
#include "Component.h"
#include "G13Container.h"

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

	int renderTimer;
	int inputTimer;

	void CreateContainers();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool BTN0;
	bool BTN1;
	bool BTN2;
	bool BTN3;

	std::vector<Component*> components;
};

