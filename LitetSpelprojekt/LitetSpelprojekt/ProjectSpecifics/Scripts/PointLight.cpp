#include "PointLight.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

PointLight::PointLight(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	targetTransform(nullptr)
{
}

PointLight::~PointLight()
{
}

void PointLight::setTarget(GameObject& targetObject)
{
	this->targetTransform = targetObject.getComponent<Transform>();
}

void PointLight::init()
{
	this->transform = this->getTransform();
}

void PointLight::update()
{
	// Rotation
	Vector3 forwardVec = this->targetTransform->getPosition() -
		this->transform->getPosition();
	forwardVec.Normalize();
	this->transform->setRotation(forwardVec, Vector3(0, 1, 0));
}

void PointLight::onCollisionEnter(GameObject& other)
{
}

void PointLight::onCollisionStay(GameObject& other)
{
}

void PointLight::onCollisionExit(GameObject& other)
{
}
