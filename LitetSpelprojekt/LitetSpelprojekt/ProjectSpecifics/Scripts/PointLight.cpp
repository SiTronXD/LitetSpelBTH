#include "PointLight.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PointLight::PointLight(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	targetTransform(nullptr),
	rotateTimer(0.0f)
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
	this->transform->setScaling(3.0f, 3.0f, 3.0f);
}

void PointLight::update()
{
	// Rotation
	this->rotateTimer += Time::getDT();
	Vector3 forwardVec = this->targetTransform->getPosition() -
		this->transform->getPosition();
	forwardVec.Normalize();

	Vector3 rotatingUp = XMVector3Transform(
		Vector3(0, 1, 0),
		XMMatrixRotationNormal(forwardVec, this->rotateTimer)
	);

	this->transform->setRotation(forwardVec, rotatingUp);
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
