#pragma once

#include <Windows.h>
#include <string>
#include <LogitechLCDLib.h>
#include <vector>
#include <map>

#include "Tools.h"
#include "G13.h"


#define RENDER_TIMER	1001
#define INPUT_TIMER		1002
#define API_POLL_TIMER	1003

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

	std::map<std::string, Component*> components;

	virtual void Render();
	virtual void OnAPITimer() {};
	virtual void OnBtnChange() {};

private:

	int renderTimer;
	int inputTimer;
	int apiPollTimer;

	bool oldBTN0 = false;
	bool oldBTN1 = false;
	bool oldBTN2 = false;
	bool oldBTN3 = false;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CheckInput();

	G13Container backBuffer;
	G13Container frontBuffer;
};

