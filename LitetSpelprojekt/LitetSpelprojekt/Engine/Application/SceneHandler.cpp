#include "SceneHandler.h"

SceneHandler::SceneHandler()
	:scene(nullptr)
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
	}
	this->scene = scene;
}

Scene* SceneHandler::getScene() const
{
	return this->scene;
}
