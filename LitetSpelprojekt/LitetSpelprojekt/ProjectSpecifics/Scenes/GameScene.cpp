#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/GameObject.h"

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
