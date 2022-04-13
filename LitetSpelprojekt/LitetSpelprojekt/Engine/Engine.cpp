#include "Engine.h"
#include "Dev/Log.h"
#include "Time.h"

// Temp
#include "ECS.h"

Engine::Engine()
	: renderer(this->resources),
	uiRenderer(this->renderer, this->resources)
{
	this->settings.loadSettings();
	this->window.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY, "Litet Spelprojekt");
	this->renderer.init(this->window);
	this->resources.init(&this->renderer);
	this->uiRenderer.init(this->settings.getSettings().resolutionX, this->settings.getSettings().resolutionY);

	this->resources.addTexture("Resources/Textures/me.png", "me.png");
	this->resources.addMaterial("me.png", "testMaterial");
	this->resources.addMesh(
		MeshData(DefaultMesh::CUBE), 
		"CubeMesh"
	);
}

Engine::~Engine()
{
}

void Engine::update(float dt)
{
}

void Engine::run()
{
	ECS ecs;
	GameObject& g = ecs.addGameObject();
	Camera tempCameraComponent(g);
	Transform* test = tempCameraComponent.getTransform();

	auto lastTime = std::chrono::high_resolution_clock::now();

	std::vector<MeshComp*> meshComponents;
	meshComponents.push_back(new MeshComp(ecs.addGameObject()));
	meshComponents.back()->setMesh("CubeMesh", "testMaterial");

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

		// Render UI
		this->uiRenderer.renderTexture("me.png", 50, 100, 300, 100);

		// Present
		this->renderer.presentSC();
	}

	for (unsigned int i = 0; i < meshComponents.size(); ++i)
		delete meshComponents[i];

	this->settings.saveSettings();
}
