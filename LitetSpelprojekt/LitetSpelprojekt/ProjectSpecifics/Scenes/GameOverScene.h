#pragma once
#include "../../Engine/Application/Scene.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/Application/SceneHandler.h"

class GameOverScene : public Scene
{
private:
	Button mainMenuButton;
	Button exitButton;

	GameObject& cam;

	bool winning;
	float highscoreTime;
public:
	GameOverScene(SceneHandler& sceneHandler, bool win, float highscoreTime);
	virtual ~GameOverScene();

	// Inherited via Scene
	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;

};

