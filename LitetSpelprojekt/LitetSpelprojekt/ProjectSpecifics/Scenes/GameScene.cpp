#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/GameObject.h"

#include "../../Engine/Time.h"

GameScene::GameScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler)
{
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
	this->getResources().addTexture("Resources/Textures/me.png", "me.png");
	this->getResources().addMaterial("me.png", "testMaterial");
	this->getResources().addMesh(
		MeshData(DefaultMesh::CUBE),
		"CubeMesh"
	);

	GameObject& cam = this->addGameObject();
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ 0.0f, 0.75f, 1.0f });
	cam.getComponent<Transform>()->rotate({ -30.0f, 0.0f, 0.0f });

	GameObject& model = this->addGameObject();
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
}

void GameScene::update()
{
}

void GameScene::renderUI()
{

}
