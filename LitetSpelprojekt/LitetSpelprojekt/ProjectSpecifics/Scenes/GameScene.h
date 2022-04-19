#pragma once

#include "../../Engine/Application/Scene.h"

class GameScene : public Scene
{
private:
public:
	GameScene(SceneHandler& sceneHandler);
	virtual ~GameScene();

	//Ui implementation
	void playerUi();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};