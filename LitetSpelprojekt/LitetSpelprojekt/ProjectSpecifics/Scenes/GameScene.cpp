#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "../Scripts/Player.h"
#include "../Scripts/GrapplingHook.h"
#include "../Scripts/GrapplingHookRope.h"
#include "../Scripts/CooldownIndicator.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"

using namespace DirectX::SimpleMath;

void GameScene::addLevelColliders(LevelLoader& levelLoader)
{
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
	: Scene(sceneHandler), cam(this->addGameObject("Player", ObjectTag::PLAYER)), currentKeys(0), keyTextTimer(0.0f), keyTextScale(0.0f),
	resumeButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, this->getUIRenderer()),
	mainMenuButton(Vector2(0, 0), 0, 0, this->getUIRenderer())
{
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
	// Text rendering
	std::vector<std::string> fontCharacterOrder =
	{
		"abcdefghij",
		"klmnopqrst",
		"uvwxyz+-.'",
		"0123456789",
		"!?,<>:()�/^",
		"@*% "
	};

	this->getResources().addTexture("Resources/Fonts/testBitmapFont.png", "fontTexture", true);
	this->getUIRenderer().setFontTexture("fontTexture");
	this->getUIRenderer().setFontCharacterOrder(
		fontCharacterOrder, 16, 16
	);
	this->getUIRenderer().setFontCharacterSpacing(5);
	this->getUIRenderer().setFontSpaceWidth(10);
	
	//Object Textures
	this->getResources().addTexture("Resources/Textures/me.png", "me.png");
	this->getResources().addTexture("Resources/Textures/RopeTexture.png", "RopeTexture.png");
	this->getResources().addTexture("Resources/Textures/StingrayPBS1SG_Base_Color_1001.png", "GrapplingHookTexture");

	//Gui textures
	this->getResources().addTexture("Resources/Textures/Gui/crosshairs64.png", "crosshairs64.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBox.png", "HealthBox.png");
	this->getResources().addTexture("Resources/Textures/Gui/HealthBar.png", "HealthBar.png");
	this->getResources().addTexture("Resources/Textures/Gui/TimerBox.png", "TimerBox.png");
	this->getResources().addTexture("Resources/Textures/Gui/EmptyKeyGui.png", "EmptyKeyGui.png");
	this->getResources().addTexture("Resources/Textures/Gui/KeyGui.png", "KeyGui.png");
	this->getResources().addTexture("Resources/Textures/WhiteTexture.png", "WhiteTexture.png");

	//this->getResources().addTexture("Resources/Textures/GemTexture.png", "GemTexture.png");
	//this->getResources().addTexture("Resources/Textures/portalTexture.jpg", "portalTexture.jpg");

	//Menu textures
	this->getResources().addTexture("Resources/Textures/MenuGui/PauseMenu.png", "PauseMenu.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/NeatBox.png", "NeatBox.png");
	
	//Materials
	this->getResources().addMaterial("me.png", "testMaterial");
	this->getResources().addMaterial("me.png", "portalMaterial");
	this->getResources().addMaterial("RopeTexture.png", "ropeMaterial");
	this->getResources().addMaterial("GrapplingHookTexture", "GrapplingHookMaterial");
	this->getResources().addMaterial("WhiteTexture.png", "WhiteMaterial");

	// Default meshes
	this->getResources().addMesh(MeshData(DefaultMesh::CUBE), "RealCubeMesh");
	this->getResources().addMesh(MeshData(DefaultMesh::SPHERE), "RealSphereMesh");
	this->getResources().addMesh(MeshData(DefaultMesh::PLANE), "PlaneMesh");
	this->getResources().addMesh(MeshData(DefaultMesh::TETRAHEDRON), "Tetrahedron");

	//Add cubemap
	this->getResources().addCubeMap("SkyBox", ".bmp", "skybox");
	this->getRenderer().setSkyBoxName("skybox");

	// Models
	MeshData testMeshData = MeshLoader::loadModel("Resources/Models/suzanne.obj");
	testMeshData.transformMesh(
		Matrix::CreateScale(0.5f, 1.0f, 1.0f) * Matrix::CreateRotationZ(3.14f * 0.3f)
	);
	this->getResources().addMesh(
		std::move(testMeshData), //MeshData(DefaultMesh::CUBE),
		"CubeMesh" 
	);
	MeshData grapplingHookData = MeshLoader::loadModel("Resources/Models/MeyerWeaponOBJEdit.obj");
	this->getResources().addMesh(
		std::move(grapplingHookData),
		"GrapplingHookMesh"
	);
	MeshData cooldownIndicatorMeshData(DefaultMesh::PLANE);
	cooldownIndicatorMeshData.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5f));
	this->getResources().addMesh(
		std::move(cooldownIndicatorMeshData), 
		"CooldownIndicatorMesh"
	);

	MeshData ropeMesh(DefaultMesh::LOW_POLY_CYLINDER);
	ropeMesh.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5f));
	this->getResources().addMesh(
		std::move(ropeMesh),
		"RopeMesh"
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
  
	// Player
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos() + Vector3(0,10,0)});
	cam.addComponent<Player>();
	cam.addComponent<Rigidbody>();
	Collider* col = cam.addComponent<Collider>();
	col->setSphereCollider(0.5f);
	cam.getComponent<Camera>()->updateAspectRatio(
		(float) this->getWindow().getWidth() / this->getWindow().getHeight()
	);

	// Origin
	GameObject& origin = this->addGameObject("Origin");
	origin.getComponent<Transform>()->setScaling(Vector3(3,3,3));
	MeshComp* originMC = origin.addComponent<MeshComp>();
	originMC->setMesh("RealSphereMesh", "testMaterial");

	// Grappling hook
	GameObject& grapplingHook = this->addGameObject("Grappling hook");
	AbsoluteMeshComp* amc = grapplingHook.addComponent<AbsoluteMeshComp>();
	amc->setMesh("GrapplingHookMesh", "GrapplingHookMaterial");
	GrapplingHook* grapplingHookComp = 
		grapplingHook.addComponent<GrapplingHook>();
	grapplingHookComp->setPlayerTransform(cam.getComponent<Transform>());

	// Grappling hook rope
	GameObject& rope = this->addGameObject("Rope");
	rope.getComponent<Transform>()->setPosition(Vector3(2, -8, 0));
	MeshComp* mc = rope.addComponent<MeshComp>();
	mc->setMesh("RopeMesh", "ropeMaterial");
	GrapplingHookRope* grapplingHookRopeComp =
		rope.addComponent<GrapplingHookRope>();
	grapplingHookRopeComp->setGrapplingHook(grapplingHookComp);

	// Grappling hook cooldown indicator
	GameObject& cooldownIndicatorObject = this->addGameObject("Grappling Hook Cooldown Indicator");
	amc = cooldownIndicatorObject.addComponent<AbsoluteMeshComp>();
	amc->setMesh("CooldownIndicatorMesh", "WhiteMaterial");
	CooldownIndicator* cooldownIndicatorComp =
		cooldownIndicatorObject.addComponent<CooldownIndicator>();
	cooldownIndicatorComp->setup(grapplingHookComp);

	GameObject& model = this->addGameObject("Suzanne1");
	model.getComponent<Transform>()->setScaling(5.0f, 5.0f, 5.0f);
	Rigidbody* rb = model.addComponent<Rigidbody>();
	rb->addForce(Vector3(0, 2, -2));
	mc = model.addComponent<MeshComp>();
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
	mc->setMesh("PlaneMesh", "testMaterial");
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
		keyMc->setMesh("RealCubeMesh", "testMaterial");
		Collider* keyCol = portalKey.addComponent<Collider>();
		keyCol->setBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		portalKey.getComponent<Transform>()->setScaling({ 0.6f, 0.6f, 0.6f });
		portalKey.getComponent<Transform>()->setPosition((5.0f + (4 * i)), -9.0f, 2.0f);

		this->portalKeys.push_back(&portalKey);
	}

	//Portal
	GameObject& portal = this->addGameObject("Portal", ObjectTag::PORTAL);
	MeshComp* portalMc = portal.addComponent<MeshComp>();
	portalMc->setMesh("RealCubeMesh", "portalMaterial");
	Collider* portalCol = portal.addComponent<Collider>();
	portalCol->setBoxCollider(Vector3(2.0f, 4.0f, 1.0f));
	portal.getComponent<Transform>()->setScaling({ 4.0f, 8.0f, 1.0f });
	portal.getComponent<Transform>()->setPosition(-6.0f, -6.0f, -8.0f);

	//Test obstacle, taking damage etc
	for (int i = 0; i < 3; i++)
	{
		GameObject& enemy = this->addGameObject("Enemy", ObjectTag::ENEMY);
		MeshComp* enemyMc = enemy.addComponent<MeshComp>();
		enemyMc->setMesh("Tetrahedron", "testMaterial");
		Collider* enemyCollider = enemy.addComponent<Collider>();
		enemyCollider->setBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		enemy.getComponent<Transform>()->setScaling({ 1.0f, 1.0f, 1.0f });
		enemy.getComponent<Transform>()->setPosition((5.0f + (4 * i)), -9.0f, -6.0f);

		this->enemies.push_back(&enemy);

	}

	//Buttons
	this->resumeButton.setPos(Vector2(0, 300));
	this->resumeButton.setWidth(354);
	this->resumeButton.setHeight(159);

	this->mainMenuButton.setPos(Vector2(0, 150));
	this->mainMenuButton.setWidth(354);
	this->mainMenuButton.setHeight(159);

	this->exitButton.setPos(Vector2(0, 0));
	this->exitButton.setWidth(354);
	this->exitButton.setHeight(159);

	this->getECS().init();
}

#include <iostream>
void GameScene::update()
{
	if (this->getPause() == false)
	{
		//When they player pick up a key.
		if (this->cam.getComponent<Player>()->isKeyPickUp())
		{
			std::cout << "Key pickup!" << std::endl;
			this->keyTextScale = 0.0f;
			this->keyTextTimer = 200.0f;
		}

		//Check if player is dead or not
		if (this->cam.getComponent<Player>()->isPlayerDead())
		{
			std::cout << "Player is dead NOOB!!" << std::endl;
			this->getSceneHandler().setScene(new GameOverScene(this->getSceneHandler(), false));
		}

		this->currentKeys = this->cam.getComponent<Player>()->getCurrentKeys();

		//Player enters the portal and win.
		if (this->cam.getComponent<Player>()->onPortal() && this->currentKeys >= 4)
		{
			std::cout << "YOU HAVE WON!!" << std::endl;
			this->getSceneHandler().setScene(new GameOverScene(this->getSceneHandler(), true));
		}

		//Text scaling effect
		if (this->keyTextTimer > 0.0f)
		{
			this->keyTextTimer -= (120.0f * Time::getDT());

			if (this->keyTextScale < 64.0f)
				this->keyTextScale += (150.0f * Time::getDT());
		}
	}
	else
	{
		//Buttons
		if (this->resumeButton.isClicked())
		{
			Input::setLockCursorPosition(true);
			Input::setCursorVisible(false);
			this->setPause(false);
		}

		/*if (this->mainMenuButton.isClicked())
			this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));*/
				
		if (this->exitButton.isClicked())
			this->getWindow().quit();
	}

	//Pause
	if (Input::isKeyJustPressed(Keys::P) && this->getPause() == true)
	{
		Input::setLockCursorPosition(true);
		Input::setCursorVisible(false);
		this->setPause(false);
	}
	else if (Input::isKeyJustPressed(Keys::P) && this->getPause() == false)
	{
		Input::setLockCursorPosition(false);
		Input::setCursorVisible(true);
		this->setPause(true);
	}
}

void GameScene::renderUI()
{
	if (this->getPause() == false)
	{
		//Crosshair
		this->getUIRenderer().renderTexture(
			"crosshairs64.png",
			0, 0, 64, 64
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
				(-949 + (83 * currentHealth)), -500, (166 * currentHealth), 50
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

		//Key pick up text
		if (this->keyTextTimer > 0)
		{
			this->getUIRenderer().renderString(
				"you picked up a key piece",
				0,
				200,
				(int)this->keyTextScale,
				(int)this->keyTextScale
			);
		}
	}
	else
	{
		this->getUIRenderer().renderTexture(
			"PauseMenu.png",
			0, 50, 600, 800
		);

		this->resumeButton.render("NeatBox.png");
		this->mainMenuButton.render("NeatBox.png");
		this->exitButton.render("NeatBox.png");
	}
}
