#include "Scene.h"
#include "SceneHandler.h"

Scene::Scene(SceneHandler& sceneHandler)
	: sceneHandler(sceneHandler),
	resources(&sceneHandler.getResources()),
	renderer(&sceneHandler.getRenderer())
{
}

Scene::~Scene()
{
}
