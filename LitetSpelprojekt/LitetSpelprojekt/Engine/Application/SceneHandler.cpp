#include "SceneHandler.h"

SceneHandler::SceneHandler(
	Resources& resources, Renderer& renderer, UIRenderer& uiRenderer,
	Window& window)
	:scene(nullptr),
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

void SceneHandler::setScene(Scene* scene)
{
	if (this->scene != nullptr)
	{
		delete this->scene;
		this->scene = nullptr;
	}
	this->scene = scene;
	this->scene->init();
}

Scene* SceneHandler::getScene() const
{
	return this->scene;
}
