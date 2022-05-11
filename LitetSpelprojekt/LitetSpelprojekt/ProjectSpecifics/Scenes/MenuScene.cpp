#include "MenuScene.h"
#include "LoadingScene.h"
#include "SettingsScene.h"
#include "HighscoreScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Application/Window.h"

using namespace DirectX::SimpleMath;

MenuScene::MenuScene(SceneHandler& sceneHandler) :
	Scene(sceneHandler),
	playButton(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, 0.5), Vector3(1, 1, 1), false, this->getUIRenderer()),
	highscoreButton(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, .5), Vector3(1, 1, 1), false, this->getUIRenderer()),
	settingsButton(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, 0.5), Vector3(1, 1, 1), false, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.5, 0.5, 0.5), Vector3(1, 1, 1), false, this->getUIRenderer())
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::init()
{
	this->getResources().addTexture("Resources/Textures/sliderBackground.png", "sliderBackground.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/shinyGray.png", "grayButton.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/highlightGray.png", "grayscale.png");
	

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
	this->getResources().addCubeMap("MenuBox", ".jpg", "menubox");
	this->getRenderer().setSkyBoxName("menubox");
	
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Define Play Button's size and position
	playButton.setPos(Vector2(0, 210));
	playButton.setWidth(354);
	playButton.setHeight(159);

	// Define Highscore Button's size and position
	highscoreButton.setPos(Vector2(0, 0));
	highscoreButton.setWidth(354);
	highscoreButton.setHeight(159);

	// Define Settings Button's size and position
	settingsButton.setPos(Vector2(0, -210));
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
	playButton.render("grayscale.png");
	highscoreButton.render("grayscale.png");
	settingsButton.render("grayscale.png");
	exitButton.render("grayscale.png");

	this->getUIRenderer().renderString(
		"grapple mayhem",
		-10,
		400,
		50,
		50
	);

	this->getUIRenderer().renderString(
		"play",
		-10,
		210,
		30,
		30
	);

	this->getUIRenderer().renderString(
		"highscore",
		-10,
		0,
		30,
		30
	);

	this->getUIRenderer().renderString(
		"settings",
		-10,
		-210,
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
