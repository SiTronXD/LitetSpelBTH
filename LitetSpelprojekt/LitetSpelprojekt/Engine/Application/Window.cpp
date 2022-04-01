#include "Window.h"
#include "../Dev/Log.h"

// Forward declare message handler from imgui_impl_win32.cpp
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGUI
	/*if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;*/

	switch (message)
	{
		// Key was pressed down
	case WM_KEYDOWN:
	{
		// Input::setKeyDown((unsigned int)wParam);

		break;
	}

	// Key was released
	case WM_KEYUP:
	{
		// Input::setKeyUp((unsigned int)wParam);

		break;
	}

	// Left mouse button was pressed down
	case WM_LBUTTONDOWN:
	{
		// Input::setMouseDown((unsigned int)Mouse::LEFT_BUTTON);

		break;
	}

	// Left mouse button was released
	case WM_LBUTTONUP:
	{
		// Input::setMouseUp((unsigned int)Mouse::LEFT_BUTTON);

		break;
	}

	// Right mouse button was pressed down
	case WM_RBUTTONDOWN:
	{
		// Input::setMouseDown((unsigned int)Mouse::RIGHT_BUTTON);

		break;
	}

	// Right mouse button was released
	case WM_RBUTTONUP:
	{
		// Input::setMouseUp((unsigned int)Mouse::RIGHT_BUTTON);

		break;
	}

	// Pressed exit button in window, post quit message
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;

		break;
	default:
		break;
	}

	// Default handling
	return DefWindowProc(handle, message, wParam, lParam);
}

Window::Window()
	: width(0), height(0), title(""), windowHandle(NULL),
	wc{}, running(false), cursorPoint(POINT()), lastCursorPoint(POINT())
{ }

Window::~Window()
{
	DestroyWindow(this->windowHandle);
	UnregisterClass(this->wc.lpszClassName, this->wc.hInstance);
	this->windowHandle = 0;
}

bool Window::init(unsigned int width, unsigned int height, const std::string title)
{
	this->width = width;
	this->height = height;
	this->title = title;

	// Window class
	const char CLASS_NAME[] = "MyWindowClass";

	// Settings for window class
	ZeroMemory(&this->wc, sizeof(WNDCLASS));
	this->wc.lpfnWndProc = &windowProc;
	this->wc.style = CS_HREDRAW | CS_VREDRAW;
	this->wc.hInstance = NULL;
	this->wc.lpszClassName = CLASS_NAME;
	if (!RegisterClass(&this->wc))
	{
		Log::error("Window class registration failed.");

		return false;
	}

	DWORD displayStyle = WS_OVERLAPPEDWINDOW;

	// Correct window rectangle, taking position and window borders into account
	RECT windowRect{};
	windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - (LONG)this->width) / 2;
	windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - (LONG)this->height) / 2;
	windowRect.right = windowRect.left + (LONG)this->width;
	windowRect.bottom = windowRect.top + (LONG)this->height;
	AdjustWindowRect(&windowRect, displayStyle, FALSE);

	// Create window
	this->windowHandle = CreateWindowEx(
		0, CLASS_NAME, this->title.c_str(),
		displayStyle,
		windowRect.left, windowRect.top,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, NULL, NULL
	);
	if (!this->windowHandle)
	{
		Log::error("Window handle creation failed.");

		return false;
	}

	// Display window
	ShowWindow(this->windowHandle, SW_SHOW);
	SetForegroundWindow(this->windowHandle);
	SetFocus(this->windowHandle);
	UpdateWindow(this->windowHandle);

	this->running = true;

	// Update last cursor point
	GetCursorPos(&this->lastCursorPoint);
	ScreenToClient(this->windowHandle, &this->lastCursorPoint);

	return true;
}

const bool& Window::isRunning()
{
	// Update last pressed states
	// this->input.updateLastPressedKeys();
	// this->input.updateLastPressedMouseButtons();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Handle messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Receive quit message
		if (msg.message == WM_QUIT)
			this->running = false;
	}

	// Update cursor position and delta position
	GetCursorPos(&this->cursorPoint);
	ScreenToClient(this->windowHandle, &this->cursorPoint);
	/*this->input.updateCursorPosition(this->cursorPoint.x, this->cursorPoint.y);
	this->input.updateCursorDelta(
		this->cursorPoint.x - this->lastCursorPoint.x,
		this->cursorPoint.y - this->lastCursorPoint.y
	);*/
	this->lastCursorPoint = this->cursorPoint;

	// Lock cursor position
	/*if (this->isFocus() && Input::shouldLockCursor())
	{
		// Set middle point
		this->screenMiddlePoint.x = this->width / 2;
		this->screenMiddlePoint.y = this->height / 2;
		this->lastCursorPoint = this->screenMiddlePoint;
		ClientToScreen(this->windowHandle, &screenMiddlePoint);
		SetCursorPos(screenMiddlePoint.x, screenMiddlePoint.y);
	}

	// Show/hide cursor
	if (Input::cursorVisibilitySwitched())
		ShowCursor(Input::isCursorVisible());
	this->input.updateLastCursorVisible();

	// Exit if the escape button is pressed
	if (Input::isKeyDown(Keys::ESCAPE))
		this->running = false;*/

	return this->running;
}