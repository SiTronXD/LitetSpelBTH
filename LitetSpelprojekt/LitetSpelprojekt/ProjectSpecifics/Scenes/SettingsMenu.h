#pragma once

#include "../../Engine/Application//Scene.h"
#include "../../Engine/UI/Slider.h"

class SettingsScene : public Scene
{
private:

	Slider resSlider;
	Slider sensSlider;
	Slider brightSlider;
	Slider volSlider;
public:
	SettingsScene(SceneHandler& sceneHandler);
	virtual ~SettingsScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};