#pragma once
#include "Scene.h"

class SceneHandler
{
private:
	Scene* scene;
public:
	SceneHandler();
	~SceneHandler();

	void setScene(Scene* scene);
	Scene* getScene() const;
};
