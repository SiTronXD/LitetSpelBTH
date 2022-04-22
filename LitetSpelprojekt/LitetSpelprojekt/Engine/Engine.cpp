#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"
#include "ResTranslator.h"
#include "../ProjectSpecifics/Scenes/GameScene.h"
#include "../ProjectSpecifics/Scenes/MenuScene.h"

Engine::Engine()
	: renderer(this->resources),
	uiRenderer(this->renderer, this->resources),
	sceneHandler(
		this->resources, 
		this->renderer, 
		this->uiRenderer,
		this->window
	)
{
	this->settings.loadSettings();
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
	this->renderer.init(this->window);
	this->resources.init(&this->renderer);
	this->uiRenderer.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY);

	this->sceneHandler.setScene(new MenuScene(this->sceneHandler));
	//this->sceneHandler.setScene(new GameScene(this->sceneHandler));

	// Default texture and material
	this->resources.addTexture("Resources/Textures/Default.png", "Default.png");
	this->resources.addMaterial("Default.png", "");

}

Engine::~Engine()
{
}

void Engine::run()
{
	auto lastTime = std::chrono::high_resolution_clock::now();

	Time::init();
	ResTranslator::init(this->window.getWidth(), this->window.getHeight());

	while (this->window.isRunning())
	{
		// Track delta time
		Time::updateDeltaTime();

		// ---------- Start tracking time
		lastTime = std::chrono::high_resolution_clock::now();

		// Update + render
		this->sceneHandler.updateToNextScene();
		this->sceneHandler.update();
		this->physicsEngine.updateCollisions(*this->sceneHandler.getScene());
		this->renderer.render(*this->sceneHandler.getScene());

		// ---------- Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		this->window.setTitle("Grapple Mayhem " + std::to_string((int)(1.0f / Time::getDT())) + " FPS | DT " + std::to_string(Time::getDT() * 1000.0f) + " ms");

		// Render UI
		this->sceneHandler.getScene()->renderUI();

		// Present
		this->renderer.presentSC();
	}

	this->settings.saveSettings();
}
