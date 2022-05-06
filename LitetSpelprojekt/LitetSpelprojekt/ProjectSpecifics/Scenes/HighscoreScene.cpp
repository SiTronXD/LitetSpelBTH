#include "HighscoreScene.h"
#include "MenuScene.h"

using namespace DirectX::SimpleMath;

HighscoreScene::HighscoreScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	exitButton(Vector2(0, 0), 0, 0, this->getUIRenderer())
{
}

HighscoreScene::~HighscoreScene()
{
}

void HighscoreScene::init()
{
	// Load Textures
	this->getResources().addTexture("Resources/Textures/Gui/HealthBar.png", "healthBar.png");
	this->getResources().addTexture("Resources/Textures/sliderBackground.png", "sliderBackground.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBoxTwo.png", "healthBoxTwo.png");

	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());
	this->getHighscore().displayHighscore();
	this->getHighscore().loadHighscore();
	this->getHighscore().displayHighscore();

	exitButton.setPos(Vector2(0, -420));
	exitButton.setWidth(354);
	exitButton.setHeight(159);
}

void HighscoreScene::update()
{
	this->getECS().update();

	if (exitButton.isClicked())
	{
		this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));
	}
}

void HighscoreScene::renderUI()
{
	exitButton.render("NeatBox.png");
	
	this->getUIRenderer().renderString(
		"main menu",
		0,
		-415,
		30,
		30
	);
	
	
	this->getUIRenderer().renderString(
		"highscore",
		0,
		400,
		50,
		50
	);
}