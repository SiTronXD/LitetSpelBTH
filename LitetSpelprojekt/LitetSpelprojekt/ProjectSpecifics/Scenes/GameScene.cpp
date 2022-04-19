#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
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
  
	GameObject& cam = this->addGameObject();
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ 0.0f, 0.75f, 1.0f });
	cam.getComponent<Transform>()->rotate({ -30.0f, 0.0f, 0.0f });
	cam.addComponent<Player>();

	GameObject& model = this->addGameObject();
	Rigidbody* rb = model.addComponent<Rigidbody>();
	rb->addForce(Vector3(0, 2, -2));
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");

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
