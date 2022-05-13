#include "Beam.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"

using namespace DirectX::SimpleMath;

Beam::Beam(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	targetTransform(nullptr),
	timer(0.0f)
{
}

Beam::~Beam()
{
}

void Beam::set(GameObject& keyObject, GameObject& targetObject)
{
	this->targetTransform = targetObject.getComponent<Transform>();
	this->startPos = keyObject.getComponent<Transform>()->getPosition() +
		Vector3(0.0f, 7.0f, 0.0f);
	this->startScale = Script::getTransform()->getScaling();
}

void Beam::init()
{
	this->transform = Script::getTransform();
}

void Beam::update()
{
	Vector3 newPos = this->startPos;
	Vector3 newScale = this->startScale;
	Vector3 delta = this->targetTransform->getPosition() - newPos;

	// Move within view frustum
	if (delta.LengthSquared() > this->TARGET_MAX_DIST * this->TARGET_MAX_DIST)
	{
		delta.Normalize();
		newPos = this->targetTransform->getPosition();
		newPos -= delta * this->TARGET_MAX_DIST;

		this->transform->setRotation(delta + Vector3(0, -0.7f, 0), Vector3(0, 1, 0));

		newScale.x = newScale.z = 10.0f;
		newPos.y += 70.0f;
	}
	else
		this->transform->setRotation(Vector3(1, 0, 0), Vector3(0, 1, 0));

	// Floating animation
	this->timer += Time::getDT();
	newPos.y += std::sin(this->timer) * 2.0f;

	// Set transform
	this->transform->setPosition(newPos);
	this->transform->setScaling(newScale);
}
