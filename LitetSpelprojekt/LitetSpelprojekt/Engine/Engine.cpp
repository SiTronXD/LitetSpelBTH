#include "Engine.h"

Engine::Engine(UINT width, UINT height):
	width(width), height(height)
{
	this->window.init(width, height, "Litet Spelprojekt");
	this->renderer.init(this->window);
}

Engine::~Engine()
{
}

void Engine::update(float dt)
{
}

void Engine::run()
{
	while (this->window.isRunning())
	{
		this->update(0.0f);
		this->renderer.render();
		this->renderer.presentSC();
	}
}
