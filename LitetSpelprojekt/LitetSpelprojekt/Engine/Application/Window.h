#pragma once

#include <string>
#include <Windows.h>

class Window
{
private:
	unsigned int width;
	unsigned int height;

	HWND windowHandle;
	WNDCLASS wc;

	POINT cursorPoint;
	POINT lastCursorPoint;
	POINT screenMiddlePoint;

	std::string title;

	bool running;

public:
	Window();
	~Window();

	bool init(unsigned int width, unsigned int height, const std::string title);
	const bool& isRunning();

	inline HWND& getWindowHandle() { return this->windowHandle; }
};