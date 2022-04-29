#include "MenuScene.h"
#include "LoadingScene.h"
#include "SettingsScene.h"
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
	playButton.setPos(Vector2(0, 161));
	playButton.setWidth(354);
	playButton.setHeight(159);

	// Define Settings Button's size and position
	settingsButton.setPos(Vector2(0, 0));
	settingsButton.setWidth(354);
	settingsButton.setHeight(159);

	// Define Exit Button's size and position
	exitButton.setPos(Vector2(0, -161));
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
	playButton.render("playButton.png");
	settingsButton.render("settingsButton.png");
	exitButton.render("exitButton.png");
	
	this->getUIRenderer().renderString(
		"grapple mayhem", 
		0, 
		400, 
		50, 
		50
	);
}
