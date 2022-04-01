#pragma once

#include <chrono>

#include "Application/Window.h"

class Engine
{
private:
	UINT width;
	UINT height;

	Window window;

public:
	Engine(UINT width, UINT height);
	virtual ~Engine();

	void update(float dt);
	void run();
};

