#include "Scene.h"
#include "SceneHandler.h"

Scene::Scene(SceneHandler& sceneHandler)
	: activeCamera(nullptr),
	sceneHandler(sceneHandler),
	resources(&sceneHandler.getResources()),
	renderer(&sceneHandler.getRenderer()),
	uiRenderer(&sceneHandler.getUIRenderer()),
	window(&sceneHandler.getWindow()),

	settings(&sceneHandler.getSettings())

	pause(false)
{
}

Scene::~Scene()
{
}
