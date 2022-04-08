#include "Engine.h"

Engine::Engine()
{
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
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
