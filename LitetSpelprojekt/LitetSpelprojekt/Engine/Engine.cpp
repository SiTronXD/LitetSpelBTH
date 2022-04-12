#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"

Engine::Engine()
{
	this->settings.LoadSettings();
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
	Camera tempCameraComponent((float) this->window.getWidth() / this->window.getHeight());

	auto lastTime = std::chrono::high_resolution_clock::now();

	std::vector<MeshComp*> meshComponents;
	meshComponents.push_back(new MeshComp(renderer));

	Time::init();
	while (this->window.isRunning())
	{
		// Track delta time
		Time::updateDeltaTime();


		// Start tracking time
		lastTime = std::chrono::high_resolution_clock::now();

		// Update + render
		this->update(0.0f);
		this->renderer.render(tempCameraComponent, meshComponents);

		// Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		Log::write("update + render: " + std::to_string(fp_ms.count()) + " ms");

		// Present
		this->renderer.presentSC();
	}

	for (unsigned int i = 0; i < meshComponents.size(); ++i)
		delete meshComponents[i];

	this->settings.SaveSettings();
}
