#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"
#include "ResTranslator.h"
#include "../ProjectSpecifics/Scenes/GameScene.h"

Engine::Engine()
	: renderer(this->resources),
	uiRenderer(this->renderer, this->resources),
	sceneHandler(this->resources, this->renderer)
{
	this->settings.loadSettings();
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
	this->renderer.init(this->window);
	this->resources.init(&this->renderer);
	this->uiRenderer.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY);

	this->sceneHandler.setScene(new GameScene(this->sceneHandler));
}

Engine::~Engine()
{
}

void Engine::run()
{
	auto lastTime = std::chrono::high_resolution_clock::now();

	std::vector<MeshComp*> meshComponents;
	meshComponents.push_back(new MeshComp("CubeMesh", "testMaterial"));

	Time::init();
	ResTranslator::init(this->window.getWidth(), this->window.getHeight());

	UIRectangle testRect{ -1920/2 + 100/2, 1080/2 - 100/2, 100, 100 };
	UIRectangle transformedRect = ResTranslator::transformRect(testRect);

	while (this->window.isRunning())
	{
		// Track delta time
		Time::updateDeltaTime();

		// ---------- Start tracking time
		lastTime = std::chrono::high_resolution_clock::now();

		// Update + render
		this->sceneHandler.getScene()->update();
		this->renderer.render(meshComponents);

		// ---------- Stop tracking time
		std::chrono::duration<double, std::milli> fp_ms = std::chrono::high_resolution_clock::now() - lastTime;
		Log::write("update + render: " + std::to_string(fp_ms.count()) + " ms");

		// Render UI
		this->sceneHandler.getScene()->renderUI();
		this->uiRenderer.renderTexture(
			"me.png", 
			transformedRect.x,
			transformedRect.y,
			transformedRect.width,
			transformedRect.height
		);

		// Present
		this->renderer.presentSC();
	}

	for (unsigned int i = 0; i < meshComponents.size(); ++i)
		delete meshComponents[i];

	this->settings.saveSettings();
}
