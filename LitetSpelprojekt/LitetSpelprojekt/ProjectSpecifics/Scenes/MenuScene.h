#pragma once

#include "../../Engine/Application/Scene.h"

class MenuScene : public Scene
{
private:
public:
	MenuScene(SceneHandler& sceneHandler);
	virtual ~MenuScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};

