#include "SettingsScene.h"
#include "../../Engine/GameObject.h"
#include "SimpleMath.h"
#include "../Scripts/Player.h"
using namespace DirectX::SimpleMath;

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	settingsHeader(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	brightSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	volSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, this->getUIRenderer())
{
	resolutions = {};
}

SettingsScene::~SettingsScene()
{
	
}

void SettingsScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/backgroundButton.png", "resSlider.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBar.png", "healthBar.png");
	this->getResources().addTexture("Resources/Textures/sliderBackground.png", "sliderBackground.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBox.png", "healthBox.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBoxTwo.png", "healthBoxTwo.png");

	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	resolutions.push_back("800");
	resolutions.push_back("600");
	resolutions.push_back("1280");
	resolutions.push_back("720");
	resolutions.push_back("1600");
	resolutions.push_back("900");
	resolutions.push_back("1920");
	resolutions.push_back("1080");
	resolutions.push_back("2048");
	resolutions.push_back("1080");

	float counter = 0.0f;
	switch (this->getSettings().getSettings().resolutionX)
	{
	case 800:
		counter = 0.2;
		break;
	case 1280:
		counter = 0.4;
		break;
	case 1600:
		counter = 0.6;
		break;
	case 1920:
		counter = 0.8;
		break;
	case 2048:
		counter = 1.0;
		break;
	default:
		Log::error("Unknown ResolutionX");
	}

	// Default values for sliders
	int sliderHeight = 60;
	int sliderWidth = 354;

	// Settings Header
	settingsHeader.setPos(Vector2(0, 420));
	settingsHeader.setWidth(sliderWidth);
	settingsHeader.setHeight(159);

	// Resolution Slider
	resSlider.setPos(Vector2(0, 210));
	resSlider.setWidth(sliderWidth);
	resSlider.setHeight(sliderHeight);
	resSlider.setMinVal(0.1f);
	resSlider.setCurVal(counter);
	resSlider.setMaxVal(this->resolutions.size()/2-1);
	resSlider.setPerFill(counter);

	// Sensitivity Slider
	sensSlider.setPos(Vector2(0, 70));
	sensSlider.setWidth(sliderWidth);
	sensSlider.setHeight(sliderHeight);
	sensSlider.setMinVal(0.1f);
	sensSlider.setCurVal(this->getSettings().getSettings().sensitivity);
	sensSlider.setMaxVal(1.0f);
	sensSlider.setPerFill(this->getSettings().getSettings().sensitivity);

	// Brightness Slider
	brightSlider.setPos(Vector2(0, -70));
	brightSlider.setWidth(sliderWidth);
	brightSlider.setHeight(sliderHeight);
	brightSlider.setMinVal(0.1f);
	brightSlider.setCurVal(this->getSettings().getSettings().brightness);
	brightSlider.setMaxVal(1.0f);
	brightSlider.setPerFill(this->getSettings().getSettings().brightness);

	// Volume Slider
	volSlider.setPos(Vector2(0, -210));
	volSlider.setWidth(sliderWidth);
	volSlider.setHeight(sliderHeight);
	volSlider.setMinVal(0.1f);
	volSlider.setCurVal(this->getSettings().getSettings().volume);
	volSlider.setMaxVal(1.0f);
	volSlider.setPerFill(this->getSettings().getSettings().volume);

	// Return to Menu
	exitButton.setPos(Vector2(0, -420));
	exitButton.setWidth(354);
	exitButton.setHeight(159);
}

void SettingsScene::update()
{
	this->getECS().update();
	
	if (resSlider.isClicked())
	{
		this->getSettings().getSettings().resolutionX = stoi(resolutions.at((int)this->resSlider.getCurVal() * 2));
		this->getSettings().getSettings().resolutionY = stoi(resolutions.at((int)this->resSlider.getCurVal() * 2 + 1));
		Log::write("Resolution set to: " + std::to_string(this->getSettings().getSettings().resolutionX) + "+" + std::to_string(this->getSettings().getSettings().resolutionY));
	}
	else if (sensSlider.isClicked())
	{
		this->getSettings().getSettings().sensitivity = sensSlider.getCurVal();
		Log::write("Sensitivity set to: " + std::to_string(this->getSettings().getSettings().sensitivity));
	}
	else if (brightSlider.isClicked())
	{
		this->getSettings().getSettings().brightness = brightSlider.getCurVal();
		Log::write("Brightness set to: " + std::to_string(this->getSettings().getSettings().brightness));
	}
	else if (volSlider.isClicked())
	{
		this->getSettings().getSettings().volume = volSlider.getCurVal();
		Log::write("Volume set to: " + std::to_string(this->getSettings().getSettings().volume));
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
	resSlider.render("healthBar.png");
	sensSlider.render("healthBar.png");
	brightSlider.render("healthBar.png");
	volSlider.render("healthBar.png");
	exitButton.render("exitButton.png");
}