#include <Windows.h>
#include <SimpleMath.h>
#include "GameScene.h"
#include "../../Engine/Resources.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/MeshLoader.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Graphics/UIRenderer.h"
#include "../../Engine/Physics/PhysicsEngine.h"

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
			"LevelBoxCollider: " + i
		);

		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");

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
			"LevelOrientedBoxColldier: " + i
		);
		MeshComp* mc = colliderObject.addComponent<MeshComp>();
		mc->setMesh("RealCubeMesh", "testMaterial");

		colliderObject.getComponent<Transform>()->setPosition(orientedBoxInfo.pos);
		colliderObject.getComponent<Transform>()->setRotation(orientedBoxInfo.orientation);
		colliderObject.getComponent<Transform>()->setScaling(orientedBoxInfo.extents * 2);

		Rigidbody* rb = colliderObject.addComponent<Rigidbody>();
		rb->setPhysics(this->getPhysicsEngine());
		rb->addBoxCollider(orientedBoxInfo.extents);
		rb->setType(rp3d::BodyType::STATIC);
	}
}

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
	this->getResources().addTexture("Resources/Textures/HealthBar.png", "HealthBar.png");
	this->getResources().addTexture("Resources/Textures/timergui.png", "timergui.png");
	this->getResources().addTexture("Resources/Textures/keygui.png", "keygui.png");
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
  
	GameObject& cam = this->addGameObject("Player", ObjectTag::PLAYER);
	this->setActiveCamera(cam.addComponent<Camera>());
	cam.getComponent<Transform>()->setPosition({ levelLoader.getPlayerStartPos()});
	cam.addComponent<Player>();
	Rigidbody* rb = cam.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addCapsuleCollider(1.0f, 2.0f);
	rb->setRotRestrict(Vector3(0.0f, 0.0f, 0.0f));
	rb->setMaterial(0.2f, 0.0f);

	GameObject& model = this->addGameObject("Suzanne1");
	model.getComponent<Transform>()->setScaling(5.0f, 5.0f, 5.0f);
	model.getComponent<Transform>()->setPosition(10.0f, -7.0f, 10.0f);
	rb = model.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addSphereCollider(2.0f);
	rb->setType(rp3d::BodyType::STATIC);
	MeshComp* mc = model.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");

	// Level game object
	GameObject& levelObject = this->addGameObject("LevelObject");
	MeshComp* levelMeshComponent = levelObject.addComponent<MeshComp>();
	levelMeshComponent->setMesh("LevelMesh", "testMaterial");

	GameObject& model2 = this->addGameObject("Suzanne2", ObjectTag::ENEMY);
	model2.getComponent<Transform>()->setPosition(Vector3(3, 0, 0));
	mc = model2.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");

	GameObject& ground = this->addGameObject("Ground", ObjectTag::GROUND);
	mc = ground.addComponent<MeshComp>();
	mc->setMesh("CubeMesh", "testMaterial");
	ground.getComponent<Transform>()->setScaling({ 100.0f, 1.0f, 100.0f });
	ground.getComponent<Transform>()->setPosition(0.0f, -10.0f, 0.0f);
	rb = ground.addComponent<Rigidbody>();
	rb->setPhysics(this->getPhysicsEngine());
	rb->addBoxCollider(Vector3(100.0f, 1.0f, 100.0f));
	rb->setType(rp3d::BodyType::KINEMATIC);
	rb->setMaterial(0.2f, 0.0f);

	this->getECS().init();
}

void GameScene::update()
{
	RaycastInfo info = this->getPhysicsEngine().raycast(rp3d::Ray({ 0.0f, -10.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }));
	if (info.hit)
	{
		std::cout << "Hit " << info.gameObject->getName() << ": at worldPos (" << info.hitPoint.x << ", " << info.hitPoint.y << ", " << info.hitPoint.z << ")" << std::endl;
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
		"HealthBar.png",
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
