#include "GameOverScene.h"
#include "MenuScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Application/Window.h"

using namespace DirectX::SimpleMath;

GameOverScene::GameOverScene(SceneHandler& sceneHandler, bool win)
	:Scene(sceneHandler), winning(win),
	mainMenuButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	cam(this->addGameObject("Camera"))
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::init()
{
	//Add textures
	this->getResources().addTexture("Resources/Textures/MenuGui/NeatBox.png", "NeatBox.png");
	
	//Set cubemap/skybox
	this->getResources().addCubeMap("MenuBox", ".jpg", "menubox");
	this->getRenderer().setSkyBoxName("menubox");

	//Camera
	this->setActiveCamera(this->cam.addComponent<Camera>());

	//Buttons
	this->mainMenuButton.setPos(Vector2(0, 125));
	this->mainMenuButton.setWidth(354);
	this->mainMenuButton.setHeight(159);

	this->exitButton.setPos(Vector2(0, -50));
	this->exitButton.setWidth(354);
	this->exitButton.setHeight(159);

	//Text TEMP!
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
}

void GameOverScene::update()
{
	this->cam.getComponent<Transform>()->rotate(0.0f, 0.5f, 0.0f);
	
	if (this->mainMenuButton.isClicked())
		this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));

	if (this->exitButton.isClicked())
		this->getWindow().quit();
}

void GameOverScene::renderUI()
{
	this->mainMenuButton.render("NeatBox.png");
	this->exitButton.render("NeatBox.png");

	if (this->winning)
	{
		this->getUIRenderer().renderString(
			"you won",
			0,
			400,
			64,
			64
		);
	}
	else
	{
		this->getUIRenderer().renderString(
			"you died",
			0,
			400,
			64,
			64
		);
	}
	
}
