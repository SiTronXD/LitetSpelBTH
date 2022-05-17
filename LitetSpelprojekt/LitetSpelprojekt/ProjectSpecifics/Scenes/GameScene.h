#pragma once

#include "../../Engine/Application/Scene.h"
#include "../Tools/LevelLoader.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/Application/SceneHandler.h"

class GameScene : public Scene
{
private:
	GameObject& cam;
	std::vector<GameObject*> portalKeys;
	std::vector<GameObject*> particlesObject;
	
	int currentKeys;
	float keyTextTimer;
	float keyTextScale;

	float highscoreTime;

	void addLevelProperties(
		LevelLoader& levelLoader,
		GameObject& playerGameObject
	);

	Button resumeButton;
	Button mainMenuButton;
	Button exitButton;

public:
	GameScene(SceneHandler& sceneHandler);
	virtual ~GameScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};