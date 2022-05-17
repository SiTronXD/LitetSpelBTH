#include "HighscoreScene.h"
#include "MenuScene.h"

using namespace DirectX::SimpleMath;

HighscoreScene::HighscoreScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer())
{
}

HighscoreScene::~HighscoreScene()
{
}

void HighscoreScene::init()
{
	// Set Camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	// Load in from file
	this->getHighscore().loadHighscore();
	
	// Display Highscore List in Log
	//this->getHighscore().displayHighscore();

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
	exitButton.render("buttonBackground.png");
	
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

	// Print the list
	for (int i = 0; i < 10; i++)
	{
		// Get Minutes:Seconds Format
		int seconds = this->getHighscore().getHighscoreList().times[i];
		int minutes = seconds / 60;
		int printSeconds = seconds - (minutes * 60);
		std::string minSec = std::to_string(minutes) + ":" + std::to_string(printSeconds);
		
		this->getUIRenderer().renderString(
		std::to_string((i + 1)) + ": " + minSec,
		0,
		280 - (i * 60),
		30,
		30
		);
	}		
}