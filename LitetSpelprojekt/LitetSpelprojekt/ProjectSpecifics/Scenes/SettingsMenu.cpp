#include "SettingsMenu.h"
#include "../../Engine/GameObject.h"

SettingsScene::SettingsScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	resSlider(0, 0, 0, 0, 0, this->getUIRenderer()),
	sensSlider(0, 0, 0, 0, 0, this->getUIRenderer()),
	brightSlider(0, 0, 0, 0, 0, this->getUIRenderer()),
	volSlider(0, 0, 0, 0, 0, this->getUIRenderer())

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
	GameObject& cam = this->addGameObject();
	this->setActiveCamera(cam.addComponent<Camera>());

	resSlider.setPosX(0);
	resSlider.setPosY(300);
	resSlider.setWidth(354);
	resSlider.setHeight(159);
	resSlider.setMaxVal(0);

	sensSlider.setPosX(0);
	sensSlider.setPosY(100);
	sensSlider.setWidth(354);
	sensSlider.setHeight(159);
	sensSlider.setMaxVal(0);

	brightSlider.setPosX(0);
	brightSlider.setPosY(-100);
	brightSlider.setWidth(354);
	brightSlider.setHeight(159);
	brightSlider.setMaxVal(0);

	volSlider.setPosX(0);
	volSlider.setPosY(-300);
	volSlider.setWidth(354);
	volSlider.setHeight(159);
	volSlider.setMaxVal(0);
}

void SettingsScene::update()
{
	this->getECS().update();

	// Implement Functionality
}

void SettingsScene::renderUI()
{
	resSlider.render("resSlider.png");
	sensSlider.render("resSlider.png");
	brightSlider.render("resSlider.png");
	volSlider.render("resSlider.png");
}