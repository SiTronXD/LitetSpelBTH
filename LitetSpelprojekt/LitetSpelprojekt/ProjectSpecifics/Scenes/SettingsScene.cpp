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

	resolutions.push_back("1280");
	resolutions.push_back("720");
	resolutions.push_back("1600");
	resolutions.push_back("900");
	resolutions.push_back("1920");
	resolutions.push_back("1080");
	resolutions.push_back("2048");
	resolutions.push_back("1152");
	resolutions.push_back("3840");
	resolutions.push_back("2160");

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
			Log::write("Set resolution to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
						"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.20) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.40))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at(2));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at(3));
			Log::write("Set resolution to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
						"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.40) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.60))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at(4));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at(5));
			Log::write("Set resolution to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
						"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.60) &&
				 resSlider.getCurVal() < (resSlider.getWidth() * 0.80))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at(6));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at(7));
			Log::write("Set resolution to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
						"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else if (resSlider.getCurVal() >= (resSlider.getWidth() * 0.80))
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at(8));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at(9));
			Log::write("Set resolution to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
						"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else
		{
			Log::error("Outside ResSlider");
		}
	}
	else if (sensSlider.isClicked())
	{
		float divider = sensSlider.getWidth() / 10.0;
		int range = sensSlider.getCurVal() / divider;
		switch (range)
		{
		case 0:
			this->getSettings().getSettings().sensitivity = 1;
			break;
		case 1:
			this->getSettings().getSettings().sensitivity = 2;
			break;
		case 2:
			this->getSettings().getSettings().sensitivity = 3;
			break;
		case 3:
			this->getSettings().getSettings().sensitivity = 4;
			break;
		case 4:
			this->getSettings().getSettings().sensitivity = 5;
			break;
		case 5:
			this->getSettings().getSettings().sensitivity = 6;
			break;
		case 6:
			this->getSettings().getSettings().sensitivity = 7;
			break;
		case 7:
			this->getSettings().getSettings().sensitivity = 8;
			break;
		case 8:
			this->getSettings().getSettings().sensitivity = 9;
			break;
		case 9:
			this->getSettings().getSettings().sensitivity = 10;
			break;
		default:
			Log::write("Sensitivity out of bounds");
		}
		Log::write("Sensitivity set to: " + std::to_string(this->getSettings().getSettings().sensitivity));
	}
	else if (brightSlider.isClicked())
	{
		float divider = brightSlider.getWidth() / 10.0;
		int range = brightSlider.getCurVal() / divider;
		switch (range)
		{
		case 0:
			this->getSettings().getSettings().brightness = 1;
			break;
		case 1:
			this->getSettings().getSettings().brightness = 2;
			break;
		case 2:
			this->getSettings().getSettings().brightness = 3;
			break;
		case 3:
			this->getSettings().getSettings().brightness = 4;
			break;
		case 4:
			this->getSettings().getSettings().brightness = 5;
			break;
		case 5:
			this->getSettings().getSettings().brightness = 6;
			break;
		case 6:
			this->getSettings().getSettings().brightness = 7;
			break;
		case 7:
			this->getSettings().getSettings().brightness = 8;
			break;
		case 8:
			this->getSettings().getSettings().brightness = 9;
			break;
		case 9:
			this->getSettings().getSettings().brightness = 10;
			break;
		default:
			Log::write("Brightness out of bounds.");
		}
		Log::write("Brightness set to: " + std::to_string(this->getSettings().getSettings().brightness));
	}
	else if (volSlider.isClicked())
	{
		float divider = volSlider.getWidth() / 10.0;
		int range = volSlider.getCurVal() / divider;
		switch (range)
		{
		case 0:
			this->getSettings().getSettings().volume = 1;
			break;
		case 1:
			this->getSettings().getSettings().volume = 2;
			break;
		case 2:
			this->getSettings().getSettings().volume = 3;
			break;
		case 3:
			this->getSettings().getSettings().volume = 4;
			break;
		case 4:
			this->getSettings().getSettings().volume = 5;
			break;
		case 5:
			this->getSettings().getSettings().volume = 6;
			break;
		case 6:
			this->getSettings().getSettings().volume = 7;
			break;
		case 7:
			this->getSettings().getSettings().volume = 8;
			break;
		case 8:
			this->getSettings().getSettings().volume = 9;
			break;
		case 9:
			this->getSettings().getSettings().volume = 10;
			break;
		default:
			Log::write("Volume out of bounds.");
		}
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
	resSlider.render("resSlider.png");
	sensSlider.render("resSlider.png");
	brightSlider.render("resSlider.png");
	volSlider.render("resSlider.png");
	exitButton.render("exitButton.png");
}