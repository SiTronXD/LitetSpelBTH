#pragma once

#include "../../Engine/Application/Scene.h"

class GameScene : public Scene
{
private:
	
public:
	GameScene(SceneHandler& sceneHandler);
	virtual ~GameScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};