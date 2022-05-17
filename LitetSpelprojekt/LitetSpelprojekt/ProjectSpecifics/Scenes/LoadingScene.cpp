#include "LoadingScene.h"
#include "GameScene.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/UIRenderer.h"
#include "../../Engine/Application/SceneHandler.h"

LoadingScene::LoadingScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::init()
{
	// Scene camera
	GameObject& cam = this->addGameObject("Camera");
	this->setActiveCamera(cam.addComponent<Camera>());

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

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

void LoadingScene::update()
{
}

void LoadingScene::renderUI()
{
	this->getUIRenderer().renderString(
		"loading...",
		0,
		0,
		64,
		64
	);

	// Change to Game Scene
	this->getSceneHandler().setScene(new GameScene(this->getSceneHandler()));
}
