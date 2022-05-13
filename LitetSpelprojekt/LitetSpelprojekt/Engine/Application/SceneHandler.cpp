#include "SceneHandler.h"
#include "../Ecs.h"
#include "../Time.h"

SceneHandler::SceneHandler(
	Resources& resources, Renderer& renderer, UIRenderer& uiRenderer,
	Window& window, PhysicsEngine& physicsEngine, Settings& settings, Highscore& highscore):
	scene(nullptr),
	nextScene(nullptr),
	resources(resources),
	renderer(renderer),
	uiRenderer(uiRenderer),
	window(window),
	physicsEngine(physicsEngine),
	settings(settings),
	highscore(highscore)
{
}

SceneHandler::~SceneHandler()
{
	delete this->scene;
}

void SceneHandler::update()
{
	this->scene->update();
	
	if (this->scene->getPause() == false)
		this->scene->getECS().update();
		
}

void SceneHandler::updateToNextScene()
{
	// Make sure a scene can be switched to
	if (this->nextScene != nullptr)
	{
		// Delete old scene
		delete this->scene;
		this->scene = nullptr;

		// Switch
		this->scene = this->nextScene;
		this->nextScene = nullptr;
		this->physicsEngine.init(this->scene);
		this->scene->init();
		this->scene->getECS().init();
		Time::init();
	}
}

void SceneHandler::setScene(Scene* scene)
{
	if (this->nextScene == nullptr)
	{
		this->nextScene = scene;
	}
}

Scene* SceneHandler::getScene() const
{
	return this->scene;
}
