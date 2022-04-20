#pragma once

#include <chrono>

#include "Settings.h"
#include "Application/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/UIRenderer.h"
#include "Resources.h"
#include "Application/SceneHandler.h"
#include "Physics/PhysicsEngine.h"

class Engine
{
private:
	Settings settings;
	Window window;
	Renderer renderer;
	UIRenderer uiRenderer;
	Resources resources;
	PhysicsEngine physicsEngine;

	SceneHandler sceneHandler;

public:
	Engine();
	virtual ~Engine();

	void run();
};

