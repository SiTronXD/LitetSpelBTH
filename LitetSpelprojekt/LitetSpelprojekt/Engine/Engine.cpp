#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"
#include "ResTranslator.h"
#include "../ProjectSpecifics/Scenes/GameScene.h"
#include "../ProjectSpecifics/Scenes/MenuScene.h"

// Temp
#include "ECS.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Camera.h"
#include "Components/Transform.h"

Engine::Engine()
	: renderer(this->resources),
	uiRenderer(this->renderer, this->resources),
	sceneHandler(this->resources, this->renderer, this->uiRenderer)
{
	this->settings.loadSettings();
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
	this->renderer.init(this->window);
	this->resources.init(&this->renderer);
	this->uiRenderer.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY);

	//this->sceneHandler.setScene(new GameScene(this->sceneHandler));
	this->sceneHandler.setScene(new MenuScene(this->sceneHandler));
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
		this->sceneHandler.getScene()->update();
		this->renderer.render(*this->sceneHandler.getScene());

		// ---------- Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		Log::write("update + render: " + std::to_string(fp_ms.count()) + " ms");

		// Render UI
		this->sceneHandler.getScene()->renderUI();
		/*
		this->uiRenderer.renderTexture(
			"me.png", 
			1920 / 2 - 100 / 2,
			-1080 / 2 + 100 / 2,
			100,
			100
		);
		*/
		this->sceneHandler.getScene()->renderUI();

		// Present
		this->renderer.presentSC();
	}

	this->settings.saveSettings();
}
