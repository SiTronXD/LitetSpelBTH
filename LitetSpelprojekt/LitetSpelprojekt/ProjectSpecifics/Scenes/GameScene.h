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
	std::vector<GameObject*> enemies; //Just for testing
	
	int currentKeys;
	float keyTextTimer;
	float keyTextScale;

	void addLevelColliders(LevelLoader& levelLoader);

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