#pragma once

#include "../ECS.h"
#include "../Settings.h"
#include "../Highscore.h"

class SceneHandler;
class Resources;
class Renderer;
class UIRenderer;
class Window;
class PhysicsEngine;
class AudioEngine;

class Scene
{
private:
	ECS ecs;
	Camera* activeCamera;

	SceneHandler& sceneHandler;
	Resources* resources;
	Renderer* renderer;
	UIRenderer* uiRenderer;
	Window* window;
	PhysicsEngine* physicsEngine;
	AudioEngine* audioEngine;

	Settings* settings;
	Highscore* highscore;

	bool pause;

protected:
	inline void setActiveCamera(Camera* cam) { this->activeCamera = cam; }
	inline void setPause(bool pause) { this->pause = pause; }
	inline GameObject& addGameObject(std::string name, ObjectTag tag = ObjectTag::UNTAGGED) { return this->ecs.addGameObject(name, tag); }

	inline SceneHandler& getSceneHandler() { return this->sceneHandler; }
	inline Resources& getResources() { return *this->resources; }
	inline Renderer& getRenderer() { return *this->renderer; }
	inline UIRenderer& getUIRenderer() { return *this->uiRenderer; }
	inline Window& getWindow() { return *this->window; }
	inline PhysicsEngine& getPhysicsEngine() { return *this->physicsEngine; }
	inline AudioEngine& getAudioEngine() { return *this->audioEngine; }
	inline Settings& getSettings() { return *this->settings;  }
	inline Highscore& getHighscore() { return *this->highscore; }

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
	inline bool getPause() const { return this->pause; }
};

template<typename T>
inline std::vector<T*> Scene::getActiveComponents()
{
	return this->ecs.getActiveComponents<T>();
}
