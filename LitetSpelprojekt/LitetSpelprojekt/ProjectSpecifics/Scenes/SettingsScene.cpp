#include "SettingsScene.h"
#include "../../Engine/GameObject.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	settingsHeader(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	resSlider(Vector2(0, 0), 0, 0, 0, 0, 0, this->getUIRenderer()),
	sensSlider(Vector2(0, 0), 0, 0, 0, 0, 0, this->getUIRenderer()),
	brightSlider(Vector2(0, 0), 0, 0, 0, 0, 0, this->getUIRenderer()),
	volSlider(Vector2(0,0), 0, 0, 0, 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0,0), 0, 0, this->getUIRenderer())
{
}

SettingsScene::~SettingsScene()
{
}

void SettingsScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/backgroundButton.png", "resSlider.png");
	
	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Default values for sliders
	int sliderHeight = 60;
	int sliderWidth = 354;

	// Settings Header
	settingsHeader.setPos(Vector2(0, 220));
	settingsHeader.setWidth(354);
	settingsHeader.setHeight(159);

	// Resolution Slider
	resSlider.setPos(Vector2(0, 105));
	resSlider.setWidth(sliderWidth);
	resSlider.setHeight(sliderHeight);
	resSlider.setMinVal(0);
	resSlider.setCurVal(5);
	resSlider.setMaxVal(10);

	// Sensitivity Slider
	sensSlider.setPos(Vector2(0,35));
	sensSlider.setWidth(sliderWidth);
	sensSlider.setHeight(sliderHeight);
	sensSlider.setMinVal(0);
	sensSlider.setCurVal(5);
	sensSlider.setMaxVal(35);

	// Brightness Slider
	brightSlider.setPos(Vector2(0, -35));
	brightSlider.setWidth(sliderWidth);
	brightSlider.setHeight(sliderHeight);
	brightSlider.setMinVal(0);
	brightSlider.setCurVal(5);
	brightSlider.setMaxVal(10);

	// Volume Slider
	volSlider.setPos(Vector2(0, -105));
	volSlider.setWidth(sliderWidth);
	volSlider.setHeight(sliderHeight);
	volSlider.setMinVal(0);
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

	if (exitButton.isClicked())
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