#include "Window.h"


Window::Window() {

}

Window::~Window() {

	for (std::pair<std::string, Component*> c : components)
		delete c.second;
}

void Window::Run() {

	// setup the render & input scheduler
	renderTimer = SetTimer(NULL, RENDER_TIMER, 500, NULL);
	inputTimer = SetTimer(NULL, INPUT_TIMER, 50, NULL);
	apiPollTimer = SetTimer(NULL, INPUT_TIMER, 5000, NULL);

	// create a message loop for the renderer
	MSG msg;

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

	KillTimer(NULL, renderTimer);
	KillTimer(NULL, inputTimer);
	KillTimer(NULL, apiPollTimer);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER: {

		UINT timerid = (UINT)wParam;
		UINT timerA = renderTimer;

		if (timerid == inputTimer)
			CheckInput();

		else if (timerid == renderTimer)
			Render();

		else if (timerid == apiPollTimer)
			OnAPITimer();

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

	// if any buttons have changed
	if (BTN0 != oldBTN0 || BTN1 != oldBTN1 || BTN2 != oldBTN2 || BTN3 != oldBTN3) {

		OnBtnChange();

		if (BTN0 != oldBTN0)
			OnBtn0Change(BTN0);

		if (BTN1 != oldBTN1)
			OnBtn0Change(BTN1);

		if (BTN2 != oldBTN2)
			OnBtn0Change(BTN2);

		if (BTN3 != oldBTN3)
			OnBtn0Change(BTN3);

		// make note of new button states
		oldBTN0 = BTN0;
		oldBTN1 = BTN1;
		oldBTN2 = BTN2;
		oldBTN3 = BTN3;
	}
}


void Window::Render() {

	// create container just for the screen
	G13Container screen = G13Container();

	// render all active components
	for (std::pair<std::string, Component*> c : components)
		c.second->Render(&screen);

	// send screen data to the G13
	bool worked = LogiLcdMonoSetBackground(screen.GetContainer());

	// tell G13 to update screen
	LogiLcdUpdate();
}

