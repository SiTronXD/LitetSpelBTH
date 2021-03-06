#include "Scene.h"
#include "SceneHandler.h"

Scene::Scene(SceneHandler& sceneHandler)
	: activeCamera(nullptr),
	ecs(&sceneHandler.getPhysicsEngine(), &sceneHandler.getAudioEngine()),
	sceneHandler(sceneHandler),
	resources(&sceneHandler.getResources()),
	renderer(&sceneHandler.getRenderer()),
	uiRenderer(&sceneHandler.getUIRenderer()),
	window(&sceneHandler.getWindow()),
	physicsEngine(&sceneHandler.getPhysicsEngine()),
	audioEngine(&sceneHandler.getAudioEngine()),
	settings(&sceneHandler.getSettings()),
	highscore(&sceneHandler.getHighscore()),
	pause(false)
{
}

Scene::~Scene()
{
}
