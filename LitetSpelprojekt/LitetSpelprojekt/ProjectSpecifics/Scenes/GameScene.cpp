#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "../Scripts/Player.h"
#include "../Scripts/HookPoint.h"
#include "../Scripts/GrapplingHook.h"
#include "../Scripts/GrapplingHookRope.h"
#include "../Scripts/CooldownIndicator.h"
#include "../Scripts/PointLight.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"
#include "../../Engine/Physics/PhysicsEngine.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"


using namespace DirectX::SimpleMath;

void GameScene::addLevelColliders(LevelLoader& levelLoader)
{
	// Sphere colliders
	Rigidbody* rb = nullptr;
	for (unsigned int i = 0; i < levelLoader.getSphereColliders().size(); ++i)
	{
		LevelColliderSphere sphereInfo = levelLoader.getSphereColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelSphereCollider: " + i, ObjectTag::GROUND
		);
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealSphereMesh", "testMaterial");

		colliderObject.getComponent<Transform>()->setPosition(sphereInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(Vector3(1, 1, 1) * sphereInfo.radius);

		rb = colliderObject.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->addSphereCollider(sphereInfo.radius);
		rb->setType(rp3d::BodyType::STATIC);
	}

	// Box colliders
	for (unsigned int i = 0; i < levelLoader.getBoxColliders().size(); ++i)
	{
		LevelColliderBox boxInfo = levelLoader.getBoxColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelBoxCollider: " + i, ObjectTag::GROUND
		);

		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");

		colliderObject.getComponent<Transform>()->setPosition(boxInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(boxInfo.extents * 2);

		rb = colliderObject.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->addBoxCollider(boxInfo.extents);
		rb->setType(rp3d::BodyType::STATIC);
	}

	// Oriented box colliders
	for (unsigned int i = 0; i < levelLoader.getOrientedBoxColliders().size(); ++i)
	{
		LevelColliderOrientedBox orientedBoxInfo = 
			levelLoader.getOrientedBoxColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelOrientedBoxColldier: " + i, ObjectTag::GROUND
		);
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");

		colliderObject.getComponent<Transform>()->setPosition(orientedBoxInfo.pos);
		colliderObject.getComponent<Transform>()->setRotation(orientedBoxInfo.orientation);
		colliderObject.getComponent<Transform>()->setScaling(orientedBoxInfo.extents * 2);

		rb = colliderObject.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->addBoxCollider(orientedBoxInfo.extents);
		rb->setType(rp3d::BodyType::STATIC);
	}

	// Keys
	for (unsigned int i = 0; i < levelLoader.getKeys().size(); ++i)
	{
		KeyInfo currentKeyInfo = levelLoader.getKeys()[i];
		Vector3 keyPos = currentKeyInfo.position;
		Vector3 keyColor = currentKeyInfo.color;

		//Portal key objects
		GameObject& portalKey = this->addGameObject("Key", ObjectTag::KEY);
		portalKey.addComponent<ParticleEmitter>();
		MeshComp* keyMc = portalKey.addComponent<MeshComp>();
		keyMc->setMesh("RealCubeMesh", "testMaterial");
		portalKey.getComponent<Transform>()->setScaling({ 0.6f, 0.6f, 0.6f });
		portalKey.getComponent<Transform>()->setPosition(keyPos);
		portalKey.getComponent<ParticleEmitter>()->init(this->getRenderer(), this->getResources(), 512);
		rb = portalKey.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->setType(rp3d::BodyType::STATIC);
		rb->addBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		this->portalKeys.push_back(&portalKey);

		// Point light
		GameObject& pointLightObject = this->addGameObject("Point light");
		pointLightObject.getComponent<Transform>()->setPosition(
			portalKey.getComponent<Transform>()->getPosition()
		);
		MeshComp* lightMesh = pointLightObject.addComponent<MeshComp>();
		lightMesh->setMesh("QuadMesh", "LightBloomMaterial");
		lightMesh->setColor(keyColor);
		lightMesh->setShouldShade(false);
		PointLight* pointLight = pointLightObject.addComponent<PointLight>();
		pointLight->setTarget(cam);
	}

	// Portal
	PortalInfo portalInfo = levelLoader.getPortal();
	GameObject& portal = this->addGameObject("Portal", ObjectTag::PORTAL);
	MeshComp* portalMc = portal.addComponent<MeshComp>();
	portalMc->setMesh("RealCubeMesh", "portalMaterial");
	portal.getComponent<Transform>()->setPosition(portalInfo.position);
	portal.getComponent<Transform>()->setScaling(portalInfo.scale);
	rb = portal.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->setType(rp3d::BodyType::STATIC);
	rb->addBoxCollider(portalInfo.scale * 0.5f);
}

GameScene::GameScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler),
	cam(this->addGameObject("Player", ObjectTag::PLAYER)),
	currentKeys(0),
	keyTextTimer(0.0f),
	keyTextScale(0.0f),
	highscoreTime(0.0f),
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


	//Particle texture
	this->getResources().addTexture("Resources/Textures/particle.png", "particle.png");
	this->getResources().addTexture("Resources/Textures/WhiteTexture.png", "WhiteTexture.png");
	this->getResources().addTexture("Resources/Textures/LightBloom.png", "LightBloom.png");

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
	this->getResources().addMaterial("LightBloom.png", "LightBloomMaterial");

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
	cooldownIndicatorMeshData.transformMesh(
		Matrix::CreateRotationX(SMath::PI * 0.5f) *
		Matrix::CreateRotationZ(SMath::PI)
	);
	this->getResources().addMesh(
		std::move(cooldownIndicatorMeshData), 
		"QuadMesh"
	);

	MeshData ropeMesh(DefaultMesh::LOW_POLY_CYLINDER);
	ropeMesh.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5f));
	this->getResources().addMesh(
		std::move(ropeMesh),
		"RopeMesh"
	);

	this->getResources().addMesh(
		MeshData(DefaultMesh::SPHERE),
		"SphereMesh"
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

	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos()});
	Player* player = cam.addComponent<Player>();
	player->setMouseSensitivity(this->getSettings().getSettings().sensitivity);
	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos() + Vector3(0,10,0)});
	Rigidbody* rb = cam.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addCapsuleCollider(1.0f, 2.0f);
	rb->setRotRestrict(Vector3(0.0f, 0.0f, 0.0f));
	rb->setMaterial(0.2f, 0.0f);
	cam.getComponent<Camera>()->updateAspectRatio(
		(float) this->getWindow().getWidth() / this->getWindow().getHeight()
	);

	GameObject& hookObject = this->addGameObject("HookPoint");
	HookPoint* hook = hookObject.addComponent<HookPoint>();
	hookObject.getComponent<Transform>()->setScaling(0.1f, 0.1f, 0.1f);
	rb = hookObject.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addBoxCollider(Vector3(0.25f, 0.25f, 0.25f));
	rb->setRotRestrict(Vector3(0.0f, 0.0f, 0.0f));
	rb->setMaterial(0.2f, 0.0f);
	rb->setType(rp3d::BodyType::KINEMATIC);
	rb->setTrigger(true);
	MeshComp* mc = hookObject.addComponent<MeshComp>();
	mc->setMesh("SphereMesh", "testMaterial");

	// Origin
	GameObject& origin = this->addGameObject("Origin");
	origin.getComponent<Transform>()->setScaling(Vector3(3, 3, 3));
	MeshComp* originMC = origin.addComponent<MeshComp>();
	originMC->setMesh("RealSphereMesh", "testMaterial");

	// Grappling hook rope
	GameObject& rope = this->addGameObject("Rope");
	rope.getComponent<Transform>()->setPosition(Vector3(2, -8, 0));
	mc = rope.addComponent<MeshComp>();
	mc->setMesh("RopeMesh", "ropeMaterial");
	GrapplingHookRope* grapplingHookRopeComp =
		rope.addComponent<GrapplingHookRope>();

	// Grappling hook
	GameObject& grapplingHook = this->addGameObject("Grappling hook");
	AbsoluteMeshComp* amc = grapplingHook.addComponent<AbsoluteMeshComp>();
	amc->setMesh("GrapplingHookMesh", "GrapplingHookMaterial");
	amc->setCastShadow(false);
	GrapplingHook* grapplingHookComp = 
		grapplingHook.addComponent<GrapplingHook>();
	grapplingHookComp->setRope(grapplingHookRopeComp);
	grapplingHookRopeComp->setGrapplingHook(grapplingHookComp);

	// Grappling hook cooldown indicator
	GameObject& cooldownIndicatorObject = this->addGameObject("Grappling Hook Cooldown Indicator");
	amc = cooldownIndicatorObject.addComponent<AbsoluteMeshComp>();
	amc->setMesh("QuadMesh", "WhiteMaterial");
	amc->setShouldShade(false);
	CooldownIndicator* cooldownIndicatorComp =
		cooldownIndicatorObject.addComponent<CooldownIndicator>();

	player->setGrapplingHook(hook, grapplingHookComp, cooldownIndicatorComp);

	GameObject& model = this->addGameObject("Suzanne1");
	model.getComponent<Transform>()->setScaling(5.0f, 5.0f, 5.0f);
	model.getComponent<Transform>()->setPosition(10.0f, -7.0f, 10.0f);
	rb = model.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addSphereCollider(2.0f);
	rb->setType(rp3d::BodyType::STATIC);
	mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");

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

	GameObject& ground = this->addGameObject("Ground", ObjectTag::GROUND);
	mc = ground.addComponent<MeshComp>();
	mc->setMesh("PlaneMesh", "testMaterial");
	ground.getComponent<Transform>()->setScaling({ 100.0f, 1.0f, 100.0f });
	ground.getComponent<Transform>()->setPosition(0.0f, -10.0f, 0.0f);
	rb = ground.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addBoxCollider(Vector3(100.0f, 1.0f, 100.0f));
	rb->setType(rp3d::BodyType::KINEMATIC);
	rb->setMaterial(0.2f, 0.0f);

	//Test obstacle, taking damage etc
	for (int i = 0; i < 3; i++)
	{
		GameObject& enemy = this->addGameObject("Enemy", ObjectTag::ENEMY);
		MeshComp* enemyMc = enemy.addComponent<MeshComp>();
		enemyMc->setMesh("Tetrahedron", "testMaterial");
		enemy.getComponent<Transform>()->setScaling({ 1.0f, 1.0f, 1.0f });
		enemy.getComponent<Transform>()->setPosition((5.0f + (4 * i)), -9.0f, -6.0f);
		rb = enemy.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->setType(rp3d::BodyType::KINEMATIC);
		rb->addBoxCollider(Vector3(1.0f, 1.0f, 1.0f));

		this->enemies.push_back(&enemy);
	}

	//Buttons
	this->resumeButton.setPos(Vector2(0, 170));
	this->resumeButton.setWidth(354);
	this->resumeButton.setHeight(159);

	this->mainMenuButton.setPos(Vector2(0, 0));
	this->mainMenuButton.setWidth(354);
	this->mainMenuButton.setHeight(159);

	this->exitButton.setPos(Vector2(0, -170));
	this->exitButton.setWidth(354);
	this->exitButton.setHeight(159);
}

#include <iostream>
void GameScene::update()
{
	RaycastInfo info = this->getPhysicsEngine().raycast(rp3d::Ray({ 0.0f, -10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }));
	if (info.hit)
	{
		std::cout << "Hit " << info.gameObject->getName() << ": at worldPos (" << info.hitPoint.x << ", " << info.hitPoint.y << ", " << info.hitPoint.z << ")" << std::endl;
	}
  
	if (this->getPause() == false)
	{
		Player* playerComp = cam.getComponent<Player>();
		this->highscoreTime += Time::getDT();

		//When the player picks up a key.
		if (playerComp->isKeyPickUp())
		{
			std::cout << "Key pickup!" << std::endl;
			this->keyTextScale = 0.0f;
			this->keyTextTimer = 200.0f;
		}

		//Check if player is dead or not
		if (playerComp->isPlayerDead())
		{
			std::cout << "Player is dead NOOB!!" << std::endl;
			this->getSceneHandler().setScene(new GameOverScene(this->getSceneHandler(), false, this->highscoreTime));
			Input::setLockCursorPosition(false);
			Input::setCursorVisible(true);
		}

		this->currentKeys = playerComp->getCurrentKeys();

		//Player enters the portal and win.
		if (playerComp->onPortal() && this->currentKeys >= 4)
		{
			std::cout << "YOU HAVE WON!!" << std::endl;
			this->getSceneHandler().setScene(new GameOverScene(this->getSceneHandler(), true, this->highscoreTime));
			Input::setLockCursorPosition(false);
			Input::setCursorVisible(true);
		}

		//Text scaling effect
		if (this->keyTextTimer > 0.0f)
		{
			this->keyTextTimer -= (120.0f * Time::getDT());

			if (this->keyTextScale < 64.0f)
				this->keyTextScale += (150.0f * Time::getDT());
		}

		//Partcile update
		if (Input::isKeyJustPressed(Keys::E))
		{
			std::vector<ParticleEmitter*> particleComponents = getActiveComponents<ParticleEmitter>();
			for (unsigned int i = 0; i < particleComponents.size(); ++i)
			{
				particleComponents[i]->explode(10, 1, Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f));
			}
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

		if (this->mainMenuButton.isClicked())
			this->getSceneHandler().setScene(new MenuScene(this->getSceneHandler()));
				
		if (this->exitButton.isClicked())
			this->getWindow().quit();
	}

	// Unpause
	if (Input::isKeyJustPressed(Keys::P) && this->getPause() == true)
	{
		Input::setLockCursorPosition(true);
		Input::setCursorVisible(false);
		this->setPause(false);
	} // Pause
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

		// Timer
		this->getUIRenderer().renderTexture(
			"TimerBox.png",
			-832, 420, 256, 256
		);

		// Get Minutes:Seconds Format
		int seconds = this->highscoreTime;
		int minutes = seconds / 60;
		int printSeconds = seconds - (minutes * 60);
		std::string minSec = std::to_string(minutes) + ":" + std::to_string(printSeconds);
	
		// TimerText
		this->getUIRenderer().renderString(
			(minSec),
			-845,
			410,
			35,
			35
		);

		// Key pick up text
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
		/*
		this->getUIRenderer().renderTexture(
			"PauseMenu.png",
			0, 50, 600, 800
		);
		*/

		// Resume Game
		this->resumeButton.render("NeatBox.png");
		this->getUIRenderer().renderString(
			"resume",
			-10,
			170,
			30,
			30
		);

		// Return to main menu
		this->mainMenuButton.render("NeatBox.png");
		this->getUIRenderer().renderString(
			"main menu",
			-10,
			0,
			30,
			30
		);
		
		// Exit Game
		this->exitButton.render("NeatBox.png");
		this->getUIRenderer().renderString(
			"exit",
			-10,
			-170,
			30,
			30
		);
	}
}
