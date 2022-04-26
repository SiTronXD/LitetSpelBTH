#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"

using namespace DirectX::SimpleMath;

void GameScene::addLevelColliders(LevelLoader& levelLoader)
{
	this->getResources().addMesh(MeshData(DefaultMesh::CUBE), "RealCubeMesh");
	this->getResources().addMesh(MeshData(DefaultMesh::SPHERE), "RealSphereMesh");

	// Sphere colliders
	for (unsigned int i = 0; i < levelLoader.getSphereColliders().size(); ++i)
	{
		LevelColliderSphere sphereInfo = levelLoader.getSphereColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelSphereCollider: " + i
		);
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealSphereMesh", "testMaterial");
		colliderObject.getComponent<Transform>()->setPosition(sphereInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(Vector3(1,1,1) * sphereInfo.radius);
		Collider* col = colliderObject.addComponent<Collider>();
		col->setSphereCollider(sphereInfo.radius);
	}

	// Box colliders
	for (unsigned int i = 0; i < levelLoader.getBoxColliders().size(); ++i)
	{
		LevelColliderBox boxInfo = levelLoader.getBoxColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelBoxCollider: " + i
		);

		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");
		colliderObject.getComponent<Transform>()->setPosition(boxInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(boxInfo.extents * 2);
		Collider* col = colliderObject.addComponent<Collider>();
		col->setBoxCollider(boxInfo.extents);
	}

	// Oriented box colliders
	for (unsigned int i = 0; i < levelLoader.getOrientedBoxColliders().size(); ++i)
	{
		LevelColliderOrientedBox orientedBoxInfo = 
			levelLoader.getOrientedBoxColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelOrientedBoxColldier: " + i
		);
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");
		colliderObject.getComponent<Transform>()->setPosition(orientedBoxInfo.pos);
		colliderObject.getComponent<Transform>()->setRotation(orientedBoxInfo.orientation);
		colliderObject.getComponent<Transform>()->setScaling(orientedBoxInfo.extents * 2);
		Collider* col = colliderObject.addComponent<Collider>();
		col->setOrientedBoxCollider(orientedBoxInfo.extents);
	}
}

GameScene::GameScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler), cam(this->addGameObject("Player", ObjectTag::PLAYER)), currentKeys(0)
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
	this->getResources().addTexture("Resources/Textures/Gui/crosshairs64.png", "crosshairs64.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBox.png", "HealthBox.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBar.png", "HealthBar.png");
	this->getResources().addTexture("Resources/Textures/Gui/TimerBox.png", "TimerBox.png");
	this->getResources().addTexture("Resources/Textures/Gui/EmptyKeyGui.png", "EmptyKeyGui.png");
	this->getResources().addTexture("Resources/Textures/Gui/KeyGui.png", "KeyGui.png");
	this->getResources().addTexture("Resources/Textures/Gui/GameOver.jpg", "GameOver.jpg");
	this->getResources().addMaterial("me.png", "testMaterial");

	//Add cubemap
	this->getResources().addCubeMap("SkyBox", ".bmp", "skybox");
	this->getRenderer().setSkyBoxName("skybox");

	MeshData testMeshData = MeshLoader::loadModel("Resources/Models/suzanne.obj");
	testMeshData.transformMesh(
		Matrix::CreateScale(0.5f, 1.0f, 1.0f) * Matrix::CreateRotationZ(3.14f * 0.3f)
	);
	this->getResources().addMesh(
		std::move(testMeshData), //MeshData(DefaultMesh::CUBE),
		"CubeMesh" 
	);
	this->getResources().addMesh(
		MeshData(DefaultMesh::PLANE),
		"PlaneMesh"
	);

	// Level loader
	LevelLoader levelLoader(this->getResources());
	levelLoader.load("Resources/Levels/testLevel.fbx");
	MeshData levelMeshData = levelLoader.getMeshData();
	this->getResources().addMesh(
		std::move(levelMeshData),
		"LevelMesh"
	);
	this->addLevelColliders(levelLoader);
  
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos() + Vector3(0,10,0)});
	cam.addComponent<Player>();
	cam.addComponent<Rigidbody>();
	Collider* col = cam.addComponent<Collider>();
	col->setSphereCollider(0.5f);

	GameObject& model = this->addGameObject("Suzanne1");
	model.getComponent<Transform>()->setScaling(5.0f, 5.0f, 5.0f);
	Rigidbody* rb = model.addComponent<Rigidbody>();
	rb->addForce(Vector3(0, 2, -2));
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model.addComponent<Collider>();
	col->setSphereCollider(5.0f);

	// Level game object
	GameObject& levelObject = this->addGameObject("LevelObject");
	MeshComp* levelMeshComponent = levelObject.addComponent<MeshComp>();
	levelMeshComponent->setMesh("LevelMesh", "");

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	GameObject& model2 = this->addGameObject("Suzanne2", ObjectTag::ENEMY);
	model2.getComponent<Transform>()->setPosition(Vector3(3, 0, 0));
	mc = model2.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = model2.addComponent<Collider>();
	col->setSphereCollider(1.0f);

	GameObject& ground = this->addGameObject("Ground", ObjectTag::GROUND);
	mc = ground.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	col = ground.addComponent<Collider>();
	col->setBoxCollider(Vector3(100.0f, 1.0f, 100.0f));
	rb = ground.addComponent<Rigidbody>();
	rb->setKinematicStatus(true);
	ground.getComponent<Transform>()->setScaling({ 100.0f, 1.0f, 100.0f });
	ground.getComponent<Transform>()->setPosition(0.0f, -10.0f, 0.0f);

	//Key objects
	for (int i = 0; i < 4; i++)
	{
		GameObject& portalKey = this->addGameObject("Key", ObjectTag::KEY);
		MeshComp* keyMc = portalKey.addComponent<MeshComp>();
		keyMc->setMesh("CubeMesh", "testMaterial");
		Collider* keyCol = portalKey.addComponent<Collider>();
		keyCol->setBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		portalKey.getComponent<Transform>()->setScaling({ 1.0f, 1.0f, 1.0f });
		portalKey.getComponent<Transform>()->setPosition((5.0f + (4 * i)), -9.0f, 2.0f);

		this->portalKeys.push_back(&portalKey);
	}

	//Portal
	GameObject& portal = this->addGameObject("Portal", ObjectTag::PORTAL);
	MeshComp* portalMc = portal.addComponent<MeshComp>();
	portalMc->setMesh("CubeMesh", "testMaterial");
	Collider* portalCol = portal.addComponent<Collider>();
	portalCol->setBoxCollider(Vector3(2.0f, 4.0f, 1.0f));
	portal.getComponent<Transform>()->setScaling({ 2.0f, 4.0f, 1.0f });
	portal.getComponent<Transform>()->setPosition(0.0f, -9.0f, -8.0f);

	//Test obstacle, taking damage etc
	for (int i = 0; i < 3; i++)
	{
		GameObject& enemy = this->addGameObject("Enemy", ObjectTag::ENEMY);
		MeshComp* enemyMc = enemy.addComponent<MeshComp>();
		enemyMc->setMesh("CubeMesh", "testMaterial");
		Collider* enemyCollider = enemy.addComponent<Collider>();
		enemyCollider->setBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		enemy.getComponent<Transform>()->setScaling({ 1.0f, 1.0f, 1.0f });
		enemy.getComponent<Transform>()->setPosition((5.0f + (4 * i)), -9.0f, -6.0f);

		this->enemies.push_back(&enemy);

	}

	this->getECS().init();
}

#include <iostream>
void GameScene::update()
{
	//When they player pick up a key.
	if (this->cam.getComponent<Player>()->isKeyPickUp())
	{
		std::cout << "Key pickup!" << std::endl;
	}

	//Check if player is dead or not
	if (this->cam.getComponent<Player>()->isPlayerDead())
	{
		std::cout << "Player is dead NOOB!!" << std::endl;
		//Change scene to gameover scene
	}

	this->currentKeys = this->cam.getComponent<Player>()->getCurrentKeys();

	//Player enters the portal and win.
	if (this->cam.getComponent<Player>()->onPortal() && this->currentKeys >= 4)
	{
		std::cout << "YOU HAVE WON!!" << std::endl;
		//Change scene to winning scene
	}
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
		"HealthBox.png",
		-700, -500, 500, 50
	);

	//866
	int currentHealth = this->cam.getComponent<Player>()->getHealth();
	if (currentHealth > 0)
	{
		this->getUIRenderer().renderTexture(
			"HealthBar.png",
			(- 949 + (83 * currentHealth)), -500, (166 * currentHealth), 50
		);
	}
	
	//Keys
	this->getUIRenderer().renderTexture(
		"EmptyKeyGui.png",
		800, 500, 256, 64
	);

	if (this->currentKeys > 0)
	{
		for (int i = 0; i < this->currentKeys; i++)
		{
			this->getUIRenderer().renderTexture(
				"KeyGui.png",
				(716 + (56 * i)), 500, 64, 64
			);
		}
	}

	//Timer
	this->getUIRenderer().renderTexture(
		"TimerBox.png",
		-832, 420, 256, 256
	);

	//Gameover
	/*if (this->gameOver)
	{
		this->getUIRenderer().renderTexture(
			"GameOver.jpg",
			0, 0, 1920, 1080
		);
	}*/

}
