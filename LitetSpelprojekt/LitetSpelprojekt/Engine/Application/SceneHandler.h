#pragma once
#include "Scene.h"

#include "../Resources.h"
#include "../Graphics/Renderer.h"

class SceneHandler
{
private:
	Scene* scene;

	Resources& resources;
	Renderer& renderer;

public:
	SceneHandler(Resources& resources, Renderer& renderer);
	~SceneHandler();

	void setScene(Scene* scene);
	Scene* getScene() const;

	inline Resources& getResources() { return this->resources; }
	inline Renderer& getRenderer() { return this->renderer; }
};
