#include "SceneHandler.h"

SceneHandler::SceneHandler(
	Resources& resources, Renderer& renderer, UIRenderer& uiRenderer,
	Window& window)
	:scene(nullptr),
	nextScene(nullptr),
	resources(resources),
	renderer(renderer),
	uiRenderer(uiRenderer),
	window(window)
{
}

SceneHandler::~SceneHandler()
{
	delete this->scene;
}

void SceneHandler::update()
{
	this->scene->update();
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
		this->scene->init();
	}
}

void SceneHandler::setScene(Scene* scene)
{
	if (this->nextScene == nullptr)
		this->nextScene = scene;
}

Scene* SceneHandler::getScene() const
{
	return this->scene;
}
