#include "Engine.h"
#include "Dev/Log.h"

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
	Camera tempCameraComponent;

	auto lastTime = std::chrono::high_resolution_clock::now();

	while (this->window.isRunning())
	{
		// Start tracking time
		lastTime = std::chrono::high_resolution_clock::now();

		// Update + render
		this->update(0.0f);
		this->renderer.render(tempCameraComponent);

		// Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		Log::write("update + render: " + std::to_string(fp_ms.count()) + " ms");

		// Present
		this->renderer.presentSC();
	}
}
