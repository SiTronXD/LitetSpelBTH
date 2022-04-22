#pragma once

#include "../../Engine/Application//Scene.h"
#include "../../Engine/UI/Slider.h"
#include "../../Engine/UI/Button.h"

class SettingsScene : public Scene
{
private:

	Button settingsHeader;
	Slider resSlider;
	Slider sensSlider;
	Slider brightSlider;
	Slider volSlider;
	Button exitButton;
public:
	SettingsScene(SceneHandler& sceneHandler);
	virtual ~SettingsScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};