#pragma once

#include "../ECS.h"

class SceneHandler;
class Resources;
class Renderer;

class Scene
{
private:
	ECS ecs;

	SceneHandler& sceneHandler;

	Resources* resources;
	Renderer* renderer;

protected:
	inline Resources& getResources() { return *this->resources; }
	inline Renderer& getRenderer() { return *this->renderer; }

public:
	Scene(SceneHandler& sceneHandler);
	virtual ~Scene();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void renderUI() = 0;
};