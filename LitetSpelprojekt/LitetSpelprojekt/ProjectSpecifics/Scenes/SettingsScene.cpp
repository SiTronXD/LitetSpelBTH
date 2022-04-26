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
	this->getResources().addTexture("Resources/Textures/HealthBar.png", "healthBar.png");
	this->getResources().addTexture("Resources/Textures/sliderBackground.png", "sliderBackground.png");

	resolutions.push_back("800");
	resolutions.push_back("600");
	resolutions.push_back("1280");
	resolutions.push_back("840");
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

	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

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
	resSlider.setMinVal(0.2);
	resSlider.setCurVal(counter);
	resSlider.setMaxVal(1.0);
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
			this->getSettings().getSettings().sensitivity = 0.1f;
			break;
		case 1:
			this->getSettings().getSettings().sensitivity = 0.2f;
			break;
		case 2:
			this->getSettings().getSettings().sensitivity = 0.3f;
			break;
		case 3:
			this->getSettings().getSettings().sensitivity = 0.4f;
			break;
		case 4:
			this->getSettings().getSettings().sensitivity = 0.5f;
			break;
		case 5:
			this->getSettings().getSettings().sensitivity = 0.6f;
			break;
		case 6:
			this->getSettings().getSettings().sensitivity = 0.7f;
			break;
		case 7:
			this->getSettings().getSettings().sensitivity = 0.8f;
			break;
		case 8:
			this->getSettings().getSettings().sensitivity = 0.9f;
			break;
		case 9:
			this->getSettings().getSettings().sensitivity = 1.0f;
			break;
		default:
			Log::write("Sensitivity out of bounds");
		}
		Log::write("Sensitivity set to: " + std::to_string(this->getSettings().getSettings().sensitivity));
	}
	else if (brightSlider.isClicked())
	{
		float divider = brightSlider.getWidth() / 10.0f;
		int range = brightSlider.getCurVal() / divider;
		switch (range)
		{
		case 0:
			this->getSettings().getSettings().brightness = 0.1f;
			break;
		case 1:
			this->getSettings().getSettings().brightness = 0.2f;
			break;
		case 2:
			this->getSettings().getSettings().brightness = 0.3f;
			break;
		case 3:
			this->getSettings().getSettings().brightness = 0.4f;
			break;
		case 4:
			this->getSettings().getSettings().brightness = 0.5f;
			break;
		case 5:
			this->getSettings().getSettings().brightness = 0.6f;
			break;
		case 6:
			this->getSettings().getSettings().brightness = 0.7f;
			break;
		case 7:
			this->getSettings().getSettings().brightness = 0.8f;
			break;
		case 8:
			this->getSettings().getSettings().brightness = 0.9f;
			break;
		case 9:
			this->getSettings().getSettings().brightness = 1.0f;
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
			this->getSettings().getSettings().volume = 0.1f;
			break;
		case 1:
			this->getSettings().getSettings().volume = 0.2f;
			break;
		case 2:
			this->getSettings().getSettings().volume = 0.3f;
			break;
		case 3:
			this->getSettings().getSettings().volume = 0.4f;
			break;
		case 4:
			this->getSettings().getSettings().volume = 0.5f;
			break;
		case 5:
			this->getSettings().getSettings().volume = 0.6f;
			break;
		case 6:
			this->getSettings().getSettings().volume = 0.7f;
			break;
		case 7:
			this->getSettings().getSettings().volume = 0.8f;
			break;
		case 8:
			this->getSettings().getSettings().volume = 0.9f;
			break;
		case 9:
			this->getSettings().getSettings().volume = 1.0f;
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
	resSlider.render("sliderBackground.png");
	sensSlider.render("sliderBackground.png");
	brightSlider.render("sliderBackground.png");
	volSlider.render("sliderBackground.png");
	exitButton.render("exitButton.png");
}