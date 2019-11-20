#include "Window.h"



Window::Window()
{
	// setup the render & input scheduler
	SetTimer(NULL, RENDER_TIMER, 500, NULL);
	SetTimer(NULL, INPUT_TIMER, 500, NULL);

	// create a message loop for the renderer
	MSG msg;

	testNum = 0;

	// keep looping until we get the quit message
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	while (msg.message != WM_QUIT) {
		// cause this thread to wait until there is a message to process
		if (GetMessage(&msg, NULL, 0, 0)) {

			// translate virtual key messages into char messages
			TranslateMessage(&msg);

			// pass the message information along to the event handler
			WndProc(NULL, msg.message, msg.wParam, msg.lParam);

			// dispach message (not sure if needed any more but still here just in case)
			DispatchMessage(&msg);
		}
	}
}

Window::~Window()
{
	KillTimer(NULL, RENDER_TIMER);
	KillTimer(NULL, INPUT_TIMER);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER: {

		if (wParam == INPUT_TIMER)
			CheckInput();

		else if (wParam == RENDER_TIMER)
			Render();

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::CheckInput() {

	// check if buttons have been pressed
	BTN0 = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_0);
	BTN1 = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1);
	BTN2 = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2);
	BTN3 = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_3);
}

void Window::Render() {

	// render to screen
	std::wstring pingus = L"Rendering!" + (Tools::to_Wstring(std::to_string(testNum)));
	LogiLcdMonoSetText(0, &pingus[0]);

	LogiLcdUpdate();
	testNum++;

	if (testNum > 10)
		PostMessage(NULL, WM_CLOSE, 0, 0);
}