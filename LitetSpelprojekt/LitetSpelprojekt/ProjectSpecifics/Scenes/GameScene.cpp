#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"

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



	//this->getRenderer().setActiveCamera(camera);
}

void GameScene::update()
{

}

void GameScene::renderUI()
{

}
