#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"
#include "ResTranslator.h"
#include "../ProjectSpecifics/Scenes/MenuScene.h"

// Temp
#include "../ProjectSpecifics/Scenes/GameScene.h"
#include "../ProjectSpecifics/Scenes/SettingsScene.h"
#include "../ProjectSpecifics/Scenes/GameOverScene.h"

Engine::Engine()
	: renderer(this->resources),
	uiRenderer(this->renderer, this->resources),
	sceneHandler(
		this->resources, 
		this->renderer, 
		this->uiRenderer,
		this->window,
		this->physicsEngine,
		this->settings,
		this->highscore
	)
{
	
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
	this->renderer.init(this->window);
	this->resources.init(&this->renderer);
	this->uiRenderer.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY);
	this->highscore.loadHighscore();
	
	this->sceneHandler.setScene(new MenuScene(this->sceneHandler));
	//this->sceneHandler.setScene(new GameScene(this->sceneHandler));
	//this->sceneHandler.setScene(new GameOverScene(this->sceneHandler, false, 0.0f));
	
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
		this->physicsEngine.update();
		this->renderer.render(*this->sceneHandler.getScene());

		// ---------- Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		this->window.setTitle("Grapple Mayhem " + std::to_string((int)(1.0f / Time::getDT())) + " FPS | DT " + std::to_string(Time::getDT() * 1000.0f) + " ms");

		// Render UI
		this->sceneHandler.getScene()->renderUI();

		// Present
		this->renderer.presentSC();
	}
}
