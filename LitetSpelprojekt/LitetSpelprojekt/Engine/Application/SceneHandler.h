#pragma once
#include "Scene.h"

#include "../Resources.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/UIRenderer.h"

class SceneHandler
{
private:
	Scene* scene;

	Resources& resources;
	Renderer& renderer;
	UIRenderer& uiRenderer;

public:
	SceneHandler(
		Resources& resources, 
		Renderer& renderer, 
		UIRenderer& uiRenderer
	);
	~SceneHandler();

	void update();

	void setScene(Scene* scene);
	Scene* getScene() const;

	inline Resources& getResources() { return this->resources; }
	inline Renderer& getRenderer() { return this->renderer; }
	inline UIRenderer& getUIRenderer() { return this->uiRenderer; }
};
