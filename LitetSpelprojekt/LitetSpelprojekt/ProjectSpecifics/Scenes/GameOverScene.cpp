#include "GameOverScene.h"
#include "MenuScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Application/Window.h"

using namespace DirectX::SimpleMath;

GameOverScene::GameOverScene(SceneHandler& sceneHandler, bool win, float highscoreTime)
	:Scene(sceneHandler),
	winning(win),
	newHighscore(false),
	highscoreTime(highscoreTime),
	mainMenuButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer(), this->getAudioEngine()),
	cam(this->addGameObject("Camera"))
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::init()
{	
	//Set cubemap/skybox
	this->getResources().addCubeMap("MenuBox", ".jpg", "menubox");
	this->getRenderer().setSkyBoxName("menubox");

	if (!this->winning)
		this->getAudioEngine().setMusic("");

	//Camera
	this->setActiveCamera(this->cam.addComponent<Camera>());

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	//Buttons
	this->mainMenuButton.setPos(Vector2(0, 0));
	this->mainMenuButton.setWidth(354);
	this->mainMenuButton.setHeight(159);

	this->exitButton.setPos(Vector2(0, -210));
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

	if (this->winning)
	{
		newHighscore = this->getHighscore().newHighscore(this->highscoreTime);
	}
	
}

void GameOverScene::update()
{
	this->cam.getComponent<Transform>()->rotate(0.0f, 0.01f, 0.0f);
	
	if (this->mainMenuButton.isClicked())
		this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));

	if (this->exitButton.isClicked())
		this->getWindow().quit();

}

void GameOverScene::renderUI()
{
	this->mainMenuButton.render("buttonBackground.png");
	this->getUIRenderer().renderString(
		"main menu",
		-10,
		0,
		30,
		30
	);

	this->exitButton.render("buttonBackground.png");
	this->getUIRenderer().renderString(
		"exit",
		-10,
		-210,
		30,
		30
	);

	if (this->winning)
	{
		this->getUIRenderer().renderString(
			"you won",
			-20,
			400,
			64,
			64
		);

		// Get Minutes:Seconds Format
		int seconds = this->highscoreTime;
		int minutes = seconds / 60;
		int printSeconds = seconds - (minutes * 60);
		std::string minSec = std::to_string(minutes) + ":" + std::to_string(printSeconds);
		
		if (newHighscore)
		{
			this->getUIRenderer().renderString(
				"new highscore!",
				0,
				170,
				30,
				30
			);
		}

		// TimerText
		this->getUIRenderer().renderString(
			("time: " + minSec),
			-10,
			240,
			50,
			50
		);
	}
	else
	{
		this->getUIRenderer().renderString(
			"you died",
			-20,
			400,
			64,
			64
		);

		// Get Minutes:Seconds Format
		int seconds = this->highscoreTime;
		int minutes = seconds / 60;
		int printSeconds = seconds - (minutes * 60);
		std::string minSec = std::to_string(minutes) + ":" + std::to_string(printSeconds);

		// TimerText
		this->getUIRenderer().renderString(
			("time: " + minSec),
			0,
			170,
			50,
			50
		);
	}
}
