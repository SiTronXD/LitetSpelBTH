#include "MenuScene.h"
#include "LoadingScene.h"
#include "SettingsScene.h"
#include "HighscoreScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Application/Window.h"
#include "../../Engine/Time.h"

using namespace DirectX::SimpleMath;

MenuScene::MenuScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	playButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	highscoreButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	settingsButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	cam(this->addGameObject("Camera"))
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::init()
{
	this->getResources().addTexture("Resources/Textures/MenuGui/buttonBackground.png", "buttonBackground.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/Logo.png", "Logo.png");
	this->getAudioEngine().setMusic("Resources/SoundFiles/LonelinessOfTheWinner.wav");
	this->getResources().addSoundEffect("Resources/SoundFiles/MenuClick.wav", "MenuClick");
	
	// Text rendering
	std::vector<std::string> fontCharacterOrder =
	{
		"abcdefghij",
		"klmnopqrst",
		"uvwxyz+-.'",
		"0123456789",
		"!?,<>:()¤/^",
		"@*% "
	};

	this->getResources().addTexture("Resources/Fonts/testBitmapFont.png", "fontTexture", true);
	this->getUIRenderer().setFontTexture("fontTexture");
	this->getUIRenderer().setFontCharacterOrder(
		fontCharacterOrder, 16, 16
	);
	this->getUIRenderer().setFontCharacterSpacing(5);
	this->getUIRenderer().setFontSpaceWidth(10);
	
	//Add cubemap
	this->getResources().addCubeMap("MenuBox", ".png", "menubox");
	this->getRenderer().setSkyBoxName("menubox");

	this->setActiveCamera(this->cam.addComponent<Camera>());

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	// Define Play Button's size and position
	playButton.setPos(Vector2(0, 135));
	playButton.setWidth(354);
	playButton.setHeight(159);

	// Define Highscore Button's size and position
	highscoreButton.setPos(Vector2(0, -50));
	highscoreButton.setWidth(354);
	highscoreButton.setHeight(159);

	// Define Settings Button's size and position
	settingsButton.setPos(Vector2(0, -235));
	settingsButton.setWidth(354);
	settingsButton.setHeight(159);

	// Define Exit Button's size and position
	exitButton.setPos(Vector2(0, -420));
	exitButton.setWidth(354);
	exitButton.setHeight(159);
}

void MenuScene::update()
{
	// Check for user input
	if (playButton.isClicked())
	{
		// Change to Game Scene
		this->getSceneHandler().setScene(new LoadingScene(this->getSceneHandler()));
	}
	else if (highscoreButton.isClicked())
	{
		// Load Settings Menu
		this->getSceneHandler().setScene(new HighscoreScene(this->getSceneHandler()));
	}
	else if (settingsButton.isClicked())
	{
		// Load Settings Menu
		this->getSceneHandler().setScene(new SettingsScene(this->getSceneHandler()));
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
	playButton.render("buttonBackground.png");
	highscoreButton.render("buttonBackground.png");
	settingsButton.render("buttonBackground.png");
	exitButton.render("buttonBackground.png");

	this->getUIRenderer().renderTexture(
		"Logo.png",
		0,
		385,
		1024,
		512
	);

	this->getUIRenderer().renderString(
		"play",
		-10,
		135,
		30,
		30
	);

	this->getUIRenderer().renderString(
		"highscore",
		-10,
		-50,
		30,
		30
	);

	this->getUIRenderer().renderString(
		"settings",
		-10,
		-235,
		30,
		30
	);

	this->getUIRenderer().renderString(
		"exit",
		-10,
		-420,
		30,
		30
	);

}
