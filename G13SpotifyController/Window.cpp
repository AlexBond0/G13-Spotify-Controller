#include "Window.h"


Window::Window()
{

	CreateContainers();

	// setup the render & input scheduler
	renderTimer = SetTimer(NULL, RENDER_TIMER, 500, NULL);
	inputTimer = SetTimer(NULL, INPUT_TIMER, 500, NULL);

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
}

Window::~Window()
{
	KillTimer(NULL, renderTimer);
	KillTimer(NULL, inputTimer);
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

	G13Container screen = G13Container();

	for (Component* c : components)
		c->Render(&screen);

	/*components[1]->Render(&screen);
	components[2]->Render(&screen);
	components[3]->Render(&screen);*/

	bool worked = LogiLcdMonoSetBackground(screen.GetContainer());
	// OutputDebugString(worked ? "worked!" : "dodn't work!");

	// render to screen
	/*bool worked = LogiLcdMonoSetBackground(containers[0]->GetContainer());
	OutputDebugString(worked ? "worked!" : "dodn't work!");*/

	// tell G13 to update screen
	LogiLcdUpdate();
}

void Window::CreateContainers() {

	// components.push_back(new Component(L"exampleUI.bmp", 0, 0));

	components.push_back(new Component(L"SmallCircle.bmp", 0, 0));
	components.push_back(new Component(L"SmallCircle.bmp", 10, 12));
	components.push_back(new Component(L"SmallCircle.bmp", 5, 5));
} 