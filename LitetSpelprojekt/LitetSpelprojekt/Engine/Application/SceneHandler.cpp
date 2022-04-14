#include "SceneHandler.h"

SceneHandler::SceneHandler(
	Resources& resources, Renderer& renderer, UIRenderer& uiRenderer)
	:scene(nullptr),
	resources(resources),
	renderer(renderer),
	uiRenderer(uiRenderer)
{
}

SceneHandler::~SceneHandler()
{
	delete this->scene;
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
