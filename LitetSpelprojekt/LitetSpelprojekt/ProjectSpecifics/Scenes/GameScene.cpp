#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "../Scripts/Player.h"
#include "../Scripts/Hand.h"
#include "../Scripts/HookPoint.h"
#include "../Scripts/GrapplingHook.h"
#include "../Scripts/GrapplingHookRope.h"
#include "../Scripts/CooldownIndicator.h"
#include "../Scripts/PointLight.h"
#include "../Scripts/Key.h"
#include "../Scripts/Beam.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"
#include "../../Engine/Physics/PhysicsEngine.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"

#define RENDER_COLLIDERS

using namespace DirectX::SimpleMath;

/*
Vector3 keyArray[4]{
	Vector3(1.0f, 0.0f, 0.0f),
	Vector3(0.0f, 1.0f, 0.0f),
	Vector3(0.0f, 0.0f, 1.0f),
	Vector3(1.0f, 1.0f, 0.0f)
};
*/

void GameScene::addLevelProperties(
	LevelLoader& levelLoader,
	GameObject& playerGameObject
)
{
	// Sphere colliders
	for (unsigned int i = 0; i < levelLoader.getSphereColliders().size(); ++i)
	{
		LevelColliderSphere sphereInfo = levelLoader.getSphereColliders()[i];

		GameObject& colliderObject = this->addGameObject(
			"LevelSphereCollider: " + i, ObjectTag::GROUND
		);

#ifdef RENDER_COLLIDERS
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealSphereMesh", "testMaterial");
#endif

		colliderObject.getComponent<Transform>()->setPosition(sphereInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(Vector3(1, 1, 1) * sphereInfo.radius);

		Rigidbody* rb = colliderObject.addComponent<Rigidbody>();
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

#ifdef RENDER_COLLIDERS
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");
#endif

		colliderObject.getComponent<Transform>()->setPosition(boxInfo.pos);
		colliderObject.getComponent<Transform>()->setScaling(boxInfo.extents * 2);

		Rigidbody* rb = colliderObject.addComponent<Rigidbody>();
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

#ifdef RENDER_COLLIDERS
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");
#endif

		colliderObject.getComponent<Transform>()->setPosition(orientedBoxInfo.pos);
		colliderObject.getComponent<Transform>()->setRotation(orientedBoxInfo.orientation);
		colliderObject.getComponent<Transform>()->setScaling(orientedBoxInfo.extents * 2);

		Rigidbody* rb = colliderObject.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->addBoxCollider(orientedBoxInfo.extents);
		rb->setType(rp3d::BodyType::STATIC);
	}

	// Spikes
	for (unsigned int i = 0; i < levelLoader.getSpikes().size(); ++i)
	{
		SpikeInfo currentSpikeInfo = levelLoader.getSpikes()[i];

		GameObject& spike = this->addGameObject("Spike " + std::to_string(i), ObjectTag::ENEMY);
		MeshComp* mc = spike.addComponent<MeshComp>();
		mc->setMesh("SpikeMesh", "testMaterial");
		spike.getComponent<Transform>()->setPosition(currentSpikeInfo.position);
		spike.getComponent<Transform>()->setRotation(currentSpikeInfo.rotation);
		spike.getComponent<Transform>()->setScaling(currentSpikeInfo.scale);
		
		Rigidbody* rb = spike.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->setType(rp3d::BodyType::KINEMATIC);
		rb->addBoxCollider(currentSpikeInfo.scale * 0.5f);

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
		Rigidbody* rb = portalKey.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->setType(rp3d::BodyType::STATIC);
		rb->addBoxCollider(Vector3(1.0f, 1.0f, 1.0f));
		Key* keyScript = portalKey.addComponent<Key>();
		keyScript->setKeyColor(currentKeyInfo.color);
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

		// Beam
		GameObject& beamObject = this->addGameObject("Key beam");
		BackgroundMeshComp* beamMesh = beamObject.addComponent<BackgroundMeshComp>();
		beamMesh->setMesh("BeamMesh", "WhiteMaterial");
		beamMesh->setColor(keyColor);
		beamMesh->setPixelShaderName("Beam_PS");
		beamMesh->setShouldShade(false);
		beamMesh->setCastShadow(false);
		Beam* beamScript = beamObject.addComponent<Beam>();
		beamScript->set(portalKey, playerGameObject);

		// Set pointers in key script
		keyScript->set(&pointLightObject, &beamObject);
	}

	// Portal
	PortalInfo portalInfo = levelLoader.getPortal();
	if (portalInfo.scale.x * portalInfo.scale.y * portalInfo.scale.z > 0.0f)
	{
		GameObject& portal = this->addGameObject("Portal", ObjectTag::PORTAL);
		MeshComp* portalMc = portal.addComponent<MeshComp>();
		portalMc->setMesh("RealCubeMesh", "portalMaterial");
		portal.getComponent<Transform>()->setPosition(portalInfo.position);
		portal.getComponent<Transform>()->setScaling(portalInfo.scale);
		Rigidbody* rb = portal.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->setType(rp3d::BodyType::STATIC);
		rb->addBoxCollider(portalInfo.scale * 0.5f);
	}
}

GameScene::GameScene(SceneHandler& sceneHandler)
	: Scene(sceneHandler),
	cam(this->addGameObject("Player", ObjectTag::PLAYER)),
	currentKeys(0),
	keyTextTimer(0.0f),
	keyTextScale(0.0f),
	highscoreTime(0.0f),
	resumeButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	exitButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer()),
	mainMenuButton(Vector2(0, 0), 0, 0, Vector3(0.32, 0.27, 0.42), Vector3(0.64, 0.54, 0.84), false, this->getUIRenderer())
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
	this->getResources().addTexture("Resources/Textures/Gui/EmptyKeyGuiCross.png", "EmptyKeyGuiCross.png");
	this->getResources().addTexture("Resources/Textures/Gui/KeyGui.png", "KeyGui.png");
	this->getResources().addTexture("Resources/Textures/Gui/KeyScale.png", "KeyScale.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/settingsSlider.png", "settingsSlider.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/sliderBorder.png", "sliderBorder.png");
	this->getResources().addTexture("Resources/Textures/MenuGui/sliderBorderLong.png", "sliderBorderLong.png");
	this->getResources().addTexture("Resources/Textures/Gui/buttonBackground.png", "buttonBackground.png");

	//Particle texture
	this->getResources().addTexture("Resources/Textures/particle.png", "particle.png");
	this->getResources().addTexture("Resources/Textures/WhiteTexture.png", "WhiteTexture.png");
	this->getResources().addTexture("Resources/Textures/LightBloom.png", "LightBloom.png");

	// Sound Effects
	this->getResources().addSoundEffect("Resources/SoundFiles/PulseCannon.wav", "PulseCannon");
	this->getResources().addSoundEffect("Resources/SoundFiles/Jump.wav", "Jump");
	this->getResources().addSoundEffect("Resources/SoundFiles/HookShoot.wav", "HookShoot");
	this->getResources().addSoundEffect("Resources/SoundFiles/HookShootConnect.wav", "HookShootConnect");

	this->getAudioEngine().setMusic("");

	//this->getResources().addTexture("Resources/Textures/GemTexture.png", "GemTexture.png");
	//this->getResources().addTexture("Resources/Textures/portalTexture.jpg", "portalTexture.jpg");

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
	this->getResources().addCubeMap("SkyboxLowRes", ".png", "skybox");
	this->getRenderer().setSkyBoxName("skybox");

	// Pixel shaders
	this->getResources().addPixelShader("Beam_PS");

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

	MeshData spikeMeshData = MeshLoader::loadModel("Resources/Models/spike.obj");
	spikeMeshData.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5f));
	this->getResources().addMesh(
		std::move(spikeMeshData),
		"SpikeMesh"
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

	MeshData beamMeshData(DefaultMesh::TETRAHEDRON);
	beamMeshData.transformMesh(Matrix::CreateScale(1.0f, 100.0f, 1.0f));
	this->getResources().addMesh(
		std::move(beamMeshData),
		"BeamMesh"
	);

	MeshData handMeshData = MeshLoader::loadAnimatedModel(
		"Resources/Models/HandWithAnimation.fbx");
	this->getResources().addMesh(
		std::move(handMeshData),
		"HandMesh"
	);

	// Player
	this->setActiveCamera(cam.addComponent<Camera>());

	Player* player = cam.addComponent<Player>();
	player->setMouseSensitivity(this->getSettings().getSettings().sensitivity);
	Rigidbody* rb = cam.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addCapsuleCollider(1.0f, 2.0f);
	rb->setRotRestrict(Vector3(0.0f, 0.0f, 0.0f));
	rb->setMaterial(0.2f, 0.0f);
	cam.getComponent<Camera>()->updateAspectRatio(
		(float)this->getWindow().getWidth() / this->getWindow().getHeight()
	);

	// Level loader
	LevelLoader levelLoader(this->getResources());
	levelLoader.load("Resources/Levels/testLevelMattin2.fbx");
	MeshData levelMeshData = levelLoader.getMeshData();
	this->getResources().addMesh(
		std::move(levelMeshData),
		"LevelMesh"
	);
	this->addLevelProperties(levelLoader, cam);

	// Set player properties from level
	player->setStartPosition(levelLoader.getPlayerStartPos());
	cam.getComponent<Rigidbody>()->setPosition(levelLoader.getPlayerStartPos());

	GameObject& hookObject = this->addGameObject("HookPoint");
	HookPoint* hook = hookObject.addComponent<HookPoint>();
	hookObject.getComponent<Transform>()->setScaling(0.1f, 0.1f, 0.1f);
	/*rb = hookObject.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addBoxCollider(Vector3(0.25f, 0.25f, 0.25f));
	rb->setRotRestrict(Vector3(0.0f, 0.0f, 0.0f));
	rb->setMaterial(0.2f, 0.0f);
	rb->setType(rp3d::BodyType::KINEMATIC);*/
	MeshComp* mc = hookObject.addComponent<MeshComp>();
	mc->setMesh("SphereMesh", "testMaterial");

	// Grappling hook
	GameObject& grapplingHook = this->addGameObject("Grappling hook");
	AbsoluteMeshComp* amc = grapplingHook.addComponent<AbsoluteMeshComp>();
	amc->setMesh("GrapplingHookMesh", "GrapplingHookMaterial");
	amc->setCastShadow(false);
	GrapplingHook* grapplingHookComp = 
		grapplingHook.addComponent<GrapplingHook>();

	// Grappling hook rope
	GameObject& rope = this->addGameObject("Rope");
	rope.getComponent<Transform>()->setPosition(Vector3(2, -8, 0));
	mc = rope.addComponent<MeshComp>();
	mc->setMesh("RopeMesh", "ropeMaterial");
	GrapplingHookRope* grapplingHookRopeComp =
		rope.addComponent<GrapplingHookRope>();
	grapplingHookRopeComp->setGrapplingHook(grapplingHookComp);
	grapplingHookComp->setRope(grapplingHookRopeComp);

	// Grappling hook cooldown indicator
	GameObject& cooldownIndicatorObject = this->addGameObject("Grappling Hook Cooldown Indicator");
	amc = cooldownIndicatorObject.addComponent<AbsoluteMeshComp>();
	amc->setMesh("QuadMesh", "WhiteMaterial");
	amc->setShouldShade(false);
	CooldownIndicator* cooldownIndicatorComp =
		cooldownIndicatorObject.addComponent<CooldownIndicator>();

	// FPS hand
	GameObject handObject = this->addGameObject("FPS hand");
	MeshComp* handMesh = handObject.addComponent<MeshComp>();
	handMesh->setMesh("HandMesh", "WhiteMaterial");
	handMesh->setCastShadow(false);
	Hand* handScript = handObject.addComponent<Hand>();
	handScript->setup(this->cam);

	// Sun
	GameObject& sunObject = this->addGameObject("Sun");
	Light* lightComponent = sunObject.addComponent<Light>();
	lightComponent->init(this->getResources(), this->getRenderer());

	player->setupPointers(hook, grapplingHookComp, cooldownIndicatorComp, lightComponent);

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

		//Player fall down from a building
		if (cam.getComponent<Transform>()->getPosition().y <= 0.0f &&
			this->highscoreTime >= 0.5f)
		{
			playerComp->resetPlayer(playerComp->getStartPosition());
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

		//Particle update
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
	if (Input::isKeyJustPressed(Keys::ESCAPE) && this->getPause() == true)
	{
		Input::setLockCursorPosition(true);
		Input::setCursorVisible(false);
		this->setPause(false);
	} // Pause
	else if (Input::isKeyJustPressed(Keys::ESCAPE) && this->getPause() == false)
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

		// Display Healthbar
		int currentHealth = this->cam.getComponent<Player>()->getHealth();
		if (currentHealth > 0)
		{
			this->getUIRenderer().renderTexture(
				"settingsSlider.png",
				(-949 + (83 * currentHealth)),
				-500,
				(166 * currentHealth),
				50,
				Vector3(0.9f, 0.1f, 0.3f)
			);
		}

		// Display Healthbox
		this->getUIRenderer().renderTexture(
			"sliderBorderLong.png",
			-700, -500, 500, 50
		);

		//Keys
		this->getUIRenderer().renderTexture(
			"EmptyKeyGui.png",
			830, 480, 256, 64
		);

		if (this->currentKeys > 0)
		{
			for (int i = 0; i < this->currentKeys; i++)
			{
				this->getUIRenderer().renderTexture(
					"KeyScale.png",
					(744 + (56 * i)),
					480,
					64,
					64,
					this->cam.getComponent<Player>()->getCollectedKeyColor().at(i)
					
				);
			}
		}

		// Timer
		this->getUIRenderer().renderTexture(
			"buttonBackground.png",
			-865,
			475,
			160,
			100,
			Vector3(0.9f, 0.1f, 0.3f)
		);

		// Get Minutes:Seconds Format
		int seconds = this->highscoreTime;
		int minutes = seconds / 60;
		int printSeconds = seconds - (minutes * 60);
		std::string minSec = std::to_string(minutes) + ":" + std::to_string(printSeconds);
	
		// TimerText
		this->getUIRenderer().renderString(
			(minSec),
			-875,
			475,
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
		// Resume Game
		this->resumeButton.render("buttonBackground.png");
		this->getUIRenderer().renderString(
			"resume",
			-10,
			170,
			30,
			30
		);

		// Return to main menu
		this->mainMenuButton.render("buttonBackground.png");
		this->getUIRenderer().renderString(
			"main menu",
			-10,
			0,
			30,
			30
		);
		
		// Exit Game
		this->exitButton.render("buttonBackground.png");
		this->getUIRenderer().renderString(
			"exit",
			-10,
			-170,
			30,
			30
		);
	}
}
