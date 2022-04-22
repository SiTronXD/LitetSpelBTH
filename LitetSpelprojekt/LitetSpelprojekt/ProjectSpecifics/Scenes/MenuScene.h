#pragma once

#include "../../Engine/Application/Scene.h"
#include "../../Engine/UI/Button.h"

#include "../../Engine/Application/SceneHandler.h"
#include "GameScene.h"

class MenuScene : public Scene
{
private:
	Button playButton;
	Button settingsButton;
	Button exitButton;
public:
	MenuScene(SceneHandler& sceneHandler);
	virtual ~MenuScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};

