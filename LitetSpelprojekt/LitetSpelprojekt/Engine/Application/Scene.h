#pragma once

#include "../ECS.h"

class SceneHandler;
class Resources;
class Renderer;
class UIRenderer;

class Scene
{
private:
	ECS ecs;
	Camera* activeCamera;

	SceneHandler& sceneHandler;

	Resources* resources;
	Renderer* renderer;
	UIRenderer* uiRenderer;

protected:
	inline void setActiveCamera(Camera* cam) { this->activeCamera = cam; }
	inline GameObject& addGameObject() { return this->ecs.addGameObject(); }

	inline Resources& getResources() { return *this->resources; }
	inline Renderer& getRenderer() { return *this->renderer; }
	inline UIRenderer& getUIRenderer() { return *this->uiRenderer; }

public:
	Scene(SceneHandler& sceneHandler);
	virtual ~Scene();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void renderUI() = 0;

	inline Camera* getActiveCamera() { return this->activeCamera; }
	template<typename T>
	std::vector<T*> getActiveComponents();

	inline ECS& getECS() { return this->ecs; }
};

template<typename T>
inline std::vector<T*> Scene::getActiveComponents()
{
	return this->ecs.getActiveComponents<T>();
}
