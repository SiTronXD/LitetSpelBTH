#pragma once

#include "../../Engine/Application/Scene.h"

class LoadingScene : public Scene
{
private:
public:
    LoadingScene(SceneHandler& sceneHandler);
    virtual ~LoadingScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};
