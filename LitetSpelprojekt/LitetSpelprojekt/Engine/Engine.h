#pragma once

#include <chrono>

#include "Settings.h"
#include "Application/Window.h"
#include "Graphics/Renderer.h"

class Engine
{
private:
	Settings settings;
	Window window;
	Renderer renderer;

public:
	Engine();
	virtual ~Engine();

	void update(float dt);
	void run();
};

