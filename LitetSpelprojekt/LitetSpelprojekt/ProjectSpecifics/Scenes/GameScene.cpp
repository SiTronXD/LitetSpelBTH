#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "../Tools/LevelLoader.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"

using namespace DirectX::SimpleMath;

GameScene::GameScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler)
{
}

GameScene::~GameScene()
{
}

void GameScene::playerUi()
{
}

void GameScene::init()
{
	this->getResources().addTexture("Resources/Textures/me.png", "me.png");
	this->getResources().addTexture("Resources/Textures/crosshairs64.png", "crosshairs64.png");
	this->getResources().addTexture("Resources/Textures/healthBorder.png", "healthBorder.png");
	this->getResources().addTexture("Resources/Textures/timergui.png", "timergui.png");
	this->getResources().addTexture("Resources/Textures/keygui.png", "keygui.png");
	this->getResources().addMaterial("me.png", "testMaterial");
	

	MeshData testMeshData = MeshLoader::loadModel("Resources/Models/suzanne.obj");
	testMeshData.transformMesh(
		Matrix::CreateScale(0.5f, 1.0f, 1.0f) * Matrix::CreateRotationZ(3.14f * 0.3f)
	);
	this->getResources().addMesh(
		std::move(testMeshData), //MeshData(DefaultMesh::CUBE),
		"CubeMesh"
	);

	// Level loader
	LevelLoader levelLoader(this->getResources());
	levelLoader.load("Resources/Levels/testLevel.obj");
	MeshData levelMeshData = levelLoader.getMeshData();
	this->getResources().addMesh(
		std::move(levelMeshData),
		"LevelMesh"
	);
  
	GameObject& cam = this->addGameObject("Player", ObjectTag::PLAYER);
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos()});
	cam.getComponent<Transform>()->rotate({ -30.0f, 0.0f, 0.0f });
	cam.addComponent<Player>();
	Collider* col = cam.addComponent<Collider>();
	col->setBoxCollider(Vector3(0.5f, 0.5f, 0.5f));

	GameObject& model = this->addGameObject("Suzanne1");
	Rigidbody* rb = model.addComponent<Rigidbody>();
	rb->addForce(Vector3(0, 2, -2));
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model.addComponent<Collider>();
	col->setSphereCollider(1.0f);

	// Level game object
	GameObject& levelObject = this->addGameObject("LevelObject");
	MeshComp* levelMeshComponent = levelObject.addComponent<MeshComp>();
	levelMeshComponent->setMesh("LevelMesh", "testMaterial");

	GameObject& model2 = this->addGameObject("Suzanne2", ObjectTag::ENEMY);
	model2.getComponent<Transform>()->setPosition(Vector3(3, 0, 0));
	mc = model2.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model2.addComponent<Collider>();
	col->setSphereCollider(1.0f);
}

void GameScene::update()
{

}

void GameScene::renderUI()
{
	//Crosshair
	this->getUIRenderer().renderTexture(
		"crosshairs64.png",
		0,0,64,64
	);
	
	//Healthbar
	this->getUIRenderer().renderTexture(
		"healthBorder.png",
		-700, -500, 500, 50
	);

	//Keys
	this->getUIRenderer().renderTexture(
		"keygui.png",
		800, 500, 256, 64
	);

	//Keys
	this->getUIRenderer().renderTexture(
		"timergui.png",
		-700, 500, 256, 128
	);

	//Timer
}
