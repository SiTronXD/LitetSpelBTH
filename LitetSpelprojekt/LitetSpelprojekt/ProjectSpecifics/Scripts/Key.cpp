#include <Windows.h>
#include <SimpleMath.h>
#include "Key.h"
#include "Beam.h"
#include "../../Engine/GameObject.h"
#include "PointLight.h"

using namespace DirectX::SimpleMath;

Key::Key(GameObject& gameObject)
	: Script(gameObject),
	pointLight(nullptr),
	beam(nullptr)
{
}

Key::~Key()
{
}

void Key::set(GameObject* pointLight, GameObject* beam)
{
	this->pointLight = pointLight;
	this->beam = beam;
}

void Key::remove()
{
	// Remove mesh and rigidbody
	Script::getObject().removeComponent<MeshComp>();
	Script::getObject().removeComponent<Rigidbody>();

	// Particle system
	Script::getObject().getComponent<ParticleEmitter>()->explode(10, 1, Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f));

	// Remove point light components
	this->pointLight->removeComponent<MeshComp>();
	this->pointLight->removeComponent<PointLight>();

	// Remove beam components
	this->beam->removeComponent<BackgroundMeshComp>();
	this->beam->removeComponent<Beam>();
}

void Key::init()
{
}

void Key::update()
{
}

void Key::onCollisionEnter(GameObject& other)
{
}

void Key::onCollisionStay(GameObject& other)
{
}

void Key::onCollisionExit(GameObject& other)
{
}
