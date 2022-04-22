#include "MenuScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Application/Window.h"
using namespace DirectX::SimpleMath;

MenuScene::MenuScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	playButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	settingsButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, this->getUIRenderer())
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::init()
{
	this->getResources().addTexture("Resources/Textures/playButton.png", "playButton.png");
	this->getResources().addTexture("Resources/Textures/settingsButton.png", "settingsButton.png");
	this->getResources().addTexture("Resources/Textures/exitButton.png", "exitButton.png");
	
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());
	
	// Define Play Button's size and position
	playButton.setPos(Vector2(0, 161));
	playButton.setWidth(354);
	playButton.setHeight(159);

	// Define Settings Button's size and position
	settingsButton.setPos(Vector2(0, 0));
	settingsButton.setWidth(354);
	settingsButton.setHeight(159);

	// Define Exit Button's size and position
	exitButton.setPos(Vector2(0, - 60));
	exitButton.setWidth(354);
	exitButton.setHeight(159);
}

void MenuScene::update()
{
	// Check for user input
	if (playButton.isClicked())
	{
		// Change to Game Scene
		this->getSceneHandler().setScene(new GameScene(this->getSceneHandler()));
	}
	else if (settingsButton.isClicked())
	{
		// Load Settings Menu
		Log::error("Implement Settings Menu");
	}
	else if (exitButton.isClicked())
	{
		// Exit Game
		getWindow().quit();
	}
}

void MenuScene::renderUI()
{
	// Send the buttons to the UIRenderer to display
	playButton.render("playButton.png");
	settingsButton.render("settingsButton.png");
	exitButton.render("exitButton.png");
}
