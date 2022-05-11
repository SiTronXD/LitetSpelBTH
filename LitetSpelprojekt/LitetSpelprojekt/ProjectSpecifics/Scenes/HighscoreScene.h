#pragma once
#include "../../Engine/Application/Scene.h"
#include "../../Engine/Application/SceneHandler.h"
#include "../../Engine/UI/Button.h"

class HighscoreScene : public Scene
{
private:
	Button exitButton;
public:
	HighscoreScene(SceneHandler& sceneHandler);
	virtual ~HighscoreScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};