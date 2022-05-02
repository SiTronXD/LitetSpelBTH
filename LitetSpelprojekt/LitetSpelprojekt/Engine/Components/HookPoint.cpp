#include "HookPoint.h"
#include "../GameObject.h"
#include "../Time.h"

using namespace DirectX::SimpleMath;

HookPoint::HookPoint(GameObject& gameObject):
	Script(gameObject), rb(nullptr), shootVec(Vector3::Zero), speed(100.0f), connected(false), shotTimer(0.0f)
{
}

HookPoint::~HookPoint()
{
}

void HookPoint::shoot(const DirectX::SimpleMath::Vector3& orig, const DirectX::SimpleMath::Vector3& vec)
{
	this->rb->setPosition(orig);
	this->shootVec = vec;
	this->rb->setVelocity(this->shootVec * this->speed);
	this->shotTimer = 2.0f;
	this->connected = false;
}

bool HookPoint::isConnected() const
{
	return this->connected;
}

bool HookPoint::inMotion() const
{
	return this->shotTimer > 0.0f;
}

void HookPoint::init()
{
	this->rb = this->getObject().getComponent<Rigidbody>();
}

void HookPoint::update()
{
	if (this->inMotion())
		this->shotTimer -= Time::getDT();
}

void HookPoint::onCollisionEnter(GameObject& other)
{
	if (!this->connected && other.getTag() != ObjectTag::PLAYER)
	{
		this->connected = true;
		this->rb->setVelocity(Vector3::Zero);
	}
}

void HookPoint::onTriggerEnter(GameObject& other)
{
	if (!this->connected && other.getTag() != ObjectTag::PLAYER)
	{
		this->connected = true;
		this->rb->setVelocity(Vector3::Zero);
	}
}
