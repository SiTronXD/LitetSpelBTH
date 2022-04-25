#pragma once

#include "../../Engine/Application/Scene.h"
#include "../Tools/LevelLoader.h"

class GameScene : public Scene
{
private:

	void addLevelColliders(LevelLoader& levelLoader);

public:
	GameScene(SceneHandler& sceneHandler);
	virtual ~GameScene();

	//Ui implementation
	void playerUi();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};