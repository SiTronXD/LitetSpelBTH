#include "SettingsScene.h"
#include "../../Engine/GameObject.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	settingsHeader(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	brightSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	volSlider(Vector2(0,0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0,0), 0, 0, this->getUIRenderer())
{
	resolutions = {};
}

SettingsScene::~SettingsScene()
{
	this->getSettings().saveSettings();
}

void SettingsScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/backgroundButton.png", "resSlider.png");
	this->getResources().addTexture("Resources/Textures/HealthBar.png", "healthBar.png");

	resolutions[0] = "1280";
	resolutions[1] = "720";
	resolutions[2] = "1600";
	resolutions[3] = "900";
	resolutions[4] = "1920";
	resolutions[5] = "1080";
	resolutions[6] = "2048";
	resolutions[7] = "1152";
	resolutions[8] = "3840";
	resolutions[9] = "2160";

	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Default values for sliders
	int sliderHeight = 60;
	int sliderWidth = 354;

	// Settings Header
	settingsHeader.setPos(Vector2(0, 220));
	settingsHeader.setWidth(sliderWidth);
	settingsHeader.setHeight(159);

	// Resolution Slider
	resSlider.setPos(Vector2(0, 105));
	resSlider.setWidth(sliderWidth);
	resSlider.setHeight(sliderHeight);
	resSlider.setMinVal(1);
	resSlider.setCurVal(3);
	resSlider.setMaxVal(5);

	// Sensitivity Slider
	sensSlider.setPos(Vector2(0,35));
	sensSlider.setWidth(sliderWidth);
	sensSlider.setHeight(sliderHeight);
	sensSlider.setMinVal(1);
	sensSlider.setCurVal(5);
	sensSlider.setMaxVal(10);

	// Brightness Slider
	brightSlider.setPos(Vector2(0, -35));
	brightSlider.setWidth(sliderWidth);
	brightSlider.setHeight(sliderHeight);
	brightSlider.setMinVal(1);
	brightSlider.setCurVal(5);
	brightSlider.setMaxVal(10);

	// Volume Slider
	volSlider.setPos(Vector2(0, -105));
	volSlider.setWidth(sliderWidth);
	volSlider.setHeight(sliderHeight);
	volSlider.setMinVal(1);
	volSlider.setCurVal(5);
	volSlider.setMaxVal(10);

	// Return to Menu
	exitButton.setPos(Vector2(0, -220));
	exitButton.setWidth(354);
	exitButton.setHeight(159);
}

void SettingsScene::update()
{
	this->getECS().update();
	
	if (resSlider.isClicked())
	{
		if (resSlider.getCurVal() < (resSlider.getWidth() * 0.20))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at(0));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at(1));
			Log::write("HELLO!");
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.20) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.40))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions[2]);
			this->getSettings().getSettings().resolutionY = stoi(resolutions[3]);
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.40) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.60))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions[4]);
			this->getSettings().getSettings().resolutionY = stoi(resolutions[5]);
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.60) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.80))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions[6]);
			this->getSettings().getSettings().resolutionY = stoi(resolutions[7]);
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.80))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions[8]);
			this->getSettings().getSettings().resolutionY = stoi(resolutions[9]);
		}
		else
		{
			Log::error("Outside ResSlider");
		}
	}
	else if (sensSlider.isClicked())
	{

	}
	else if (brightSlider.isClicked())
	{

	}
	else if (volSlider.isClicked())
	{

	}
	else if (exitButton.isClicked())
	{
		// Exit Game
		this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));
	}
}

void SettingsScene::renderUI()
{
	settingsHeader.render("settingsButton.png");
	resSlider.render("resSlider.png");
	sensSlider.render("resSlider.png");
	brightSlider.render("resSlider.png");
	volSlider.render("resSlider.png");
	exitButton.render("exitButton.png");
}