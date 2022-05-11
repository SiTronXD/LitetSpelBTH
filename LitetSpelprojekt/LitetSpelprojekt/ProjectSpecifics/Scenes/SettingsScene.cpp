#include "SettingsScene.h"
#include "../../Engine/GameObject.h"
#include "SimpleMath.h"
#include "../Scripts/Player.h"
using namespace DirectX::SimpleMath;

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	settingsHeader(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, 0.5), Vector3(1, 1, 1), false, this->getUIRenderer()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	graphicsSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	volSlider(Vector2(0, 0), 0, 0, 0, 0, 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, 0.5), Vector3(1, 1, 1), false, this->getUIRenderer())
{
	resolutions = {};
}

SettingsScene::~SettingsScene()
{	
}

void SettingsScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/MenuGui/settingsSlider.png", "settingsSlider.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/sliderBorder.png", "sliderBorder.png");

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
	resSlider.setMinVal(0);
	resSlider.setCurVal(resIndex / 2.0f);
	resSlider.setMaxVal(this->resolutions.size() / 2.0f);
	resSlider.setPerFill(resIndex / (float)this->resolutions.size());

	// graphicsness™ Slider - 
	graphicsSlider.setPos(Vector2(0, 70));
	graphicsSlider.setWidth(sliderWidth);
	graphicsSlider.setHeight(sliderHeight);
	graphicsSlider.setMinVal(0.1f);
	graphicsSlider.setCurVal(this->getSettings().getSettings().graphics);
	graphicsSlider.setMaxVal(1.0f);
	graphicsSlider.setPerFill(this->getSettings().getSettings().graphics);

	// Sensitivity Slider
	sensSlider.setPos(Vector2(0, -70));
	sensSlider.setWidth(sliderWidth);
	sensSlider.setHeight(sliderHeight);
	sensSlider.setMinVal(0.1f);
	sensSlider.setCurVal(this->getSettings().getSettings().sensitivity);
	sensSlider.setMaxVal(1.0f);
	sensSlider.setPerFill(this->getSettings().getSettings().sensitivity);

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
		Log::write("Resolution set to: " + std::to_string(this->getSettings().getSettings().resolutionX) +
			"x" + std::to_string(this->getSettings().getSettings().resolutionY));
	}
	else if (sensSlider.isClicked())
	{
		this->getSettings().getSettings().sensitivity = sensSlider.getCurVal();
		Log::write("Sensitivity set to: " + std::to_string(this->getSettings().getSettings().sensitivity));
	}
	else if (graphicsSlider.isClicked())
	{
		this->getSettings().getSettings().graphics = graphicsSlider.getCurVal();
		Log::write("graphicsness set to: " + std::to_string(this->getSettings().getSettings().graphics));
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
	resSlider.render("settingsSlider.png");
	sensSlider.render("settingsSlider.png");
	graphicsSlider.render("settingsSlider.png");
	volSlider.render("settingsSlider.png");
	exitButton.render("buttonBackground.png");
	// ------------------------------------------------
	// Resolution Slider
	this->getUIRenderer().renderString(
		"resolution:",
		-10,
		260,
		30,
		30
	);

	// Resolution %
	int resPer = resSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(this->getSettings().getSettings().resolutionX) + "x" + std::to_string(this->getSettings().getSettings().resolutionY),
		300,
		210,
		30,
		30
	);

	// ------------------------------------------------
	// Graphics Slider
	this->getUIRenderer().renderString(
		"graphics:",
		-10,
		120,
		30,
		30
	);
	// Graphics %
	int graphicsPer = graphicsSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(graphicsPer) + "%",
		220,
		70,
		30,
		30
	);
	// ------------------------------------------------
	// Sensitivity Slider
	this->getUIRenderer().renderString(
		"sensitivity:",
		- 10,
		-20,
		30,
		30
	);
	// Sensitivity %
	int sensPer = sensSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(sensPer) + "%",
		220,
		-70,
		30,
		30
	);

	// Volume Slider
	this->getUIRenderer().renderString(
		"volume:",
		-10,
		-160,
		30,
		30
	);

	// Volume %
	int volPer = volSlider.getPerFill() * 100;
	this->getUIRenderer().renderString(
		std::to_string(volPer) + "%",
		220,
		-210,
		30,
		30
	);


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