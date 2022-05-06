#include "GrapplingHook.h"
#include "../../Engine/Dev/Log.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

void GrapplingHook::setToArmPos(Vector3& result)
{
	result = this->playerTransform->getPosition() +
		this->playerTransform->right() * offsetPos.x +
		this->playerTransform->up() * offsetPos.y +
		this->playerTransform->forward() * offsetPos.z;
}

GrapplingHook::GrapplingHook(GameObject& gameObject)
	: Script(gameObject),
	transform(nullptr),
	playerTransform(nullptr),
	rope(nullptr),
	offsetPos(2.1f, -1.5f, 2.0f)
{
}

GrapplingHook::~GrapplingHook()
{
}

void GrapplingHook::setPlayerTransform(Transform* playerTransform)
{
	this->playerTransform = playerTransform;
	this->setToArmPos(this->lastPos);
}

void GrapplingHook::setRope(GrapplingHookRope* rope)
{
	this->rope = rope;
}

void GrapplingHook::setToShootPos(DirectX::SimpleMath::Vector3& result)
{
	result = this->transform->getPosition();
	result += this->playerTransform->forward() * 1.5f +
		this->transform->up() * 0.5f;
}

void GrapplingHook::init()
{
	this->transform = this->getTransform();
	this->transform->setScaling(Vector3(0.5f, 0.5f, 0.5f));
}

void GrapplingHook::update()
{
	// Update position
	Vector3 nextPos;
	this->setToArmPos(nextPos);

	// Sway
	float swayT = SMath::clamp(
		Time::getDT() * this->SWAY_FACTOR,
		0.0f,
		1.0f
	);

	// Max distance
	if (Vector3::DistanceSquared(this->lastPos, nextPos) > MAX_DIST * MAX_DIST)
	{
		Vector3 delta = nextPos - this->lastPos;
		delta.Normalize();
		this->lastPos = nextPos - delta * MAX_DIST;
		swayT = 0.0f;
	}

	swayT = std::max(swayT, 0.02f);
	Vector3 newPos = Vector3::Lerp(
		this->lastPos, 
		nextPos, 
		swayT
	);
	this->transform->setPosition(newPos);

	// Update rotation
	Vector3 rotation = this->playerTransform->getRotation();
	this->transform->setRotation(
		rotation
	);

	// Update last pos
	this->lastPos = newPos;
}

void GrapplingHook::onCollisionEnter(GameObject& other)
{
}

void GrapplingHook::onCollisionStay(GameObject& other)
{
}

void GrapplingHook::onCollisionExit(GameObject& other)
{
}
