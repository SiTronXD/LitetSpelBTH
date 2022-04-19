#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

GameObject* player;

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

	MeshData testMeshData = MeshLoader::loadModel("Resources/Models/suzanne.obj");
	testMeshData.transformMesh(
		Matrix::CreateScale(0.5f, 1.0f, 1.0f) * Matrix::CreateRotationZ(3.14f * 0.3f)
	);
	this->getResources().addMesh(
		std::move(testMeshData), //MeshData(DefaultMesh::CUBE),
		"CubeMesh"
	);
  
	GameObject& cam = this->addGameObject();
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ 0.0f, 0.75f, 1.0f });
	cam.getComponent<Transform>()->rotate({ -30.0f, 0.0f, 0.0f });
	cam.addComponent<Player>();
	Collider* col = cam.addComponent<Collider>();
	col->setBoxCollider(Vector3(0.5f, 0.5f, 0.5f));
	player = &cam;

	GameObject& model = this->addGameObject();
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model.addComponent<Collider>();
	col->setSphereCollider(1.0f);

	GameObject& model2 = this->addGameObject();
	model2.getComponent<Transform>()->setPosition(Vector3(3, 0, 0));
	mc = model2.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model2.addComponent<Collider>();
	col->setSphereCollider(1.0f);
}

void GameScene::update()
{
	this->getECS().update();
}

void GameScene::renderUI()
{

}
