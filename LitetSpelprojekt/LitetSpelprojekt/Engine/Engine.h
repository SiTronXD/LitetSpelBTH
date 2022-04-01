#pragma once

#include <chrono>

#include "Application/Window.h"
#include "Graphics/Renderer.h"

class Engine
{
private:
	UINT width;
	UINT height;

	Window window;
	Renderer renderer;

public:
	Engine(UINT width, UINT height);
	virtual ~Engine();

	void update(float dt);
	void run();
};

