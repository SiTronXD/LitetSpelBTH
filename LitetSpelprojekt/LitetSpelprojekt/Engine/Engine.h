#pragma once

#include <chrono>

#include "Settings.h"
#include "Application/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/UIRenderer.h"
#include "Graphics/Renderer.h"
#include "Resources.h"
#include "Application/SceneHandler.h"
#include "Physics/PhysicsEngine.h"

class Engine
{
private:

	Window window;
	Renderer renderer;
	UIRenderer uiRenderer;
	Resources resources;
	PhysicsEngine physicsEngine;

	SceneHandler sceneHandler;

	Settings settings;
	Highscore highscore;
public:
	Engine();
	virtual ~Engine();

	void run();
};

