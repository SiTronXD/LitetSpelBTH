#include "SettingsScene.h"
#include "../../Engine/GameObject.h"
#include "SimpleMath.h"
#include "../Scripts/Player.h"
#include "../../Engine/Time.h"

using namespace DirectX::SimpleMath;

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),

	fullscreenBox(Vector2(0,0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	graphicsSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	volSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine())

	fullscreenBox(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	graphicsCycler(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	volSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer())

{
	resolutions = {};
	this->cooldown = 0.5f;
}

SettingsScene::~SettingsScene()
{	
}

void SettingsScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/MenuGui/settingsSlider.png", "settingsSlider.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/sliderBorder.png", "sliderBorder.png");
	this->getResources().addSoundEffect("Resources/SoundFiles/MenuSlider.wav", "MenuSlider");

	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	// Get supported resolutions from the renderer
	std::vector<DirectX::XMFLOAT2>& supportedResolutions =
		Scene::getRenderer().getSupportedResolutions();

	// Add supported resolutions
	for (unsigned int i = 0; i < supportedResolutions.size(); ++i)
	{
		resolutions.push_back(
			std::to_string(supportedResolutions[i].x)
		);
		resolutions.push_back(
			std::to_string(supportedResolutions[i].y)
		);
	}

	// Display current resolution in slider
	int k = 0;
	int resIndex = 0;
	for (int i = 0; i < this->resolutions.size(); i+=2)
	{
		k = i + 1;
		if (stoi(this->resolutions.at(i)) == this->getSettings().getSettings().resolutionX)
		{
			if (stoi(this->resolutions.at(k)) == this->getSettings().getSettings().resolutionY)
			{
				resIndex = i;
				
				break;
			}
		}
	}
	
	std::vector<std::string> graphicsLevels = { "low", "medium", "high" };

	// Default values for sliders
	int sliderHeight = 60;
	int sliderWidth = 354;

	// Fullscreen Checkbox
	fullscreenBox.setPos(Vector2(220, 270));
	fullscreenBox.setHeight(50);
	fullscreenBox.setWidth(50);

	// Resolution Slider
	resSlider.setPos(Vector2(0,-10));
	resSlider.setWidth(sliderWidth);
	resSlider.setHeight(sliderHeight);
	resSlider.setMinVal(0);
	resSlider.setCurVal(resIndex / 2.0f);
	resSlider.setMaxVal(this->resolutions.size() / 2.0f);
	resSlider.setPerFill(resIndex / (float)this->resolutions.size());

	// Graphicsness™ Slider
	graphicsCycler.setPos(Vector2(0, 130));
	graphicsCycler.setWidth(sliderWidth);
	graphicsCycler.setHeight(sliderHeight);
	graphicsCycler.setElements(graphicsLevels);
	graphicsCycler.setCurrent(this->getSettings().getSettings().graphics);
	

	// Sensitivity Slider
	sensSlider.setPos(Vector2(0, -150));
	sensSlider.setWidth(sliderWidth);
	sensSlider.setHeight(sliderHeight);
	sensSlider.setMinVal(0.1f);
	sensSlider.setCurVal(this->getSettings().getSettings().sensitivity);
	sensSlider.setMaxVal(1.0f);
	sensSlider.setPerFill(this->getSettings().getSettings().sensitivity);

	// Volume Slider
	volSlider.setPos(Vector2(0, -290));
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

	if (cooldown > 0)
	{
		cooldown -= Time::getDT();
	}
	else
	{
		if (resSlider.isClicked())
		{
			this->getSettings().getSettings().resolutionX = stoi(resolutions.at((int)this->resSlider.getCurVal() * 2));
			this->getSettings().getSettings().resolutionY = stoi(resolutions.at((int)this->resSlider.getCurVal() * 2 + 1));
			Log::write("Resolution set to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
				"x" + std::to_string(this->getSettings().getSettings().resolutionY));
		}
		else if (graphicsCycler.isClicked())
		{
			if (graphicsCycler.getCurrent() == "low")
			{
				graphicsCycler.setCurrent(graphicsCycler.getElements().at(1));
			}
			else if (graphicsCycler.getCurrent() == "medium")
			{
				graphicsCycler.setCurrent(graphicsCycler.getElements().at(2));
			}
			else if (graphicsCycler.getCurrent() == "high")
			{
				graphicsCycler.setCurrent(graphicsCycler.getElements().at(0));
			}

			this->getSettings().getSettings().graphics = graphicsCycler.getCurrent();
			Log::write("Graphics set to: " + this->getSettings().getSettings().graphics);
		}
		else if (sensSlider.isClicked())
		{
			this->getSettings().getSettings().sensitivity = sensSlider.getCurVal();
			Log::write("Sensitivity set to: " + std::to_string(this->getSettings().getSettings().sensitivity));
		}
		else if (volSlider.isClicked())
		{
			this->getSettings().getSettings().volume = volSlider.getCurVal();
			Log::write("Volume set to: " + std::to_string(this->getSettings().getSettings().volume));

			this->getAudioEngine().setVolume(this->getSettings().getSettings().volume);
		}
		else if (exitButton.isClicked())
		{
			// Exit Game
			this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));
		}
		else if (fullscreenBox.isClicked())
		{
			if (this->getSettings().getSettings().fullscreen == true)
			{
				this->getSettings().getSettings().fullscreen = false;
			}
			else
			{
				this->getSettings().getSettings().fullscreen = true;
			}
		}
	}
}

void SettingsScene::renderUI()
{
	fullscreenBox.render("buttonBackground.png");
	resSlider.render("settingsSlider.png");
	sensSlider.render("settingsSlider.png");
	graphicsCycler.render("buttonBackground.png");
	volSlider.render("settingsSlider.png");
	exitButton.render("buttonBackground.png");
	// ------------------------------------------------
	// Fullscreen Box
	this->getUIRenderer().renderString(
		"fullscreen:",
		-10,
		270,
		30,
		30
	);
	
	if (this->getSettings().getSettings().fullscreen == true)
	{
		this->getUIRenderer().renderString(
			"x",
			210,
			270,
			30,
			30
		);
	}
	
	// ------------------------------------------------
	// Resolution Slider
	this->getUIRenderer().renderString(
		"resolution:",
		-10,
		40,
		30,
		30
	);

	// Resolution %
	int resPer = resSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(this->getSettings().getSettings().resolutionX) + "x" + std::to_string(this->getSettings().getSettings().resolutionY),
		300,
		-10,
		30,
		30
	);

	// ------------------------------------------------
	// Graphics Slider
	this->getUIRenderer().renderString(
		"graphics:",
		-10,
		190,
		30,
		30
	);
	// Graphics level
	this->getUIRenderer().renderString(
		graphicsCycler.getCurrent(),
		-10,
		130,
		30,
		30
	);
	// ------------------------------------------------
	// 
	// Sensitivity Slider
	this->getUIRenderer().renderString(
		"sensitivity:",
		-10,
		-100,
		30,
		30
	);
	// Sensitivity %
	int sensPer = sensSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(sensPer) + "%",
		230,
		-150,
		30,
		30
	);
	// ------------------------------------------------
	
	// Volume Slider
	this->getUIRenderer().renderString(
		"volume:",
		-10,
		-240,
		30,
		30
	);

	// Volume %
	int volPer = volSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(volPer) + "%",
		230,
		-290,
		30,
		30
	);

	// -----------------------------------------------
	// Setttings Header
	this->getUIRenderer().renderString(
		"settings",
		-10,
		400,
		50,
		50
	);

	// Main Menu Text
	this->getUIRenderer().renderString(
		"main menu",
		-10,
		-415,
		30,
		30
	);
}