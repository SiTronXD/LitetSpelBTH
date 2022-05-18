#pragma once

#include "../../Engine/Application/Scene.h"
#include "../../Engine/UI/Button.h"

#include "../../Engine/Application/SceneHandler.h"

class MenuScene : public Scene
{
private:
	Button playButton;
	Button highscoreButton;
	Button settingsButton;
	Button exitButton;
	GameObject& cam;
public:
	MenuScene(SceneHandler& sceneHandler);
	virtual ~MenuScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};

