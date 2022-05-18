#pragma once
#include "../../Engine/Application/Scene.h"
#include "../../Engine/UI/Slider.h"
#include "../../Engine/UI/Button.h"
#include "../../Engine/UI/Cycler.h"
#include "../../Engine/Application/SceneHandler.h"
#include "MenuScene.h"


class SettingsScene : public Scene
{
private:
	Button fullscreenBox;
	Slider resSlider;
	Cycler graphicsCycler;
	Slider sensSlider;
	Slider volSlider;
	Button exitButton;
	std::vector<std::string>resolutions;
	float cooldown;

public:
	SettingsScene(SceneHandler& sceneHandler);
	virtual ~SettingsScene();

	virtual void init() override;
	virtual void update() override;
	virtual void renderUI() override;
};