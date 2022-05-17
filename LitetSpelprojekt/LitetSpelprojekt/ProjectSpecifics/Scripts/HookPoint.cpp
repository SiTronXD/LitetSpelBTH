#include "HookPoint.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "../../Engine/Components/Rigidbody.h"
#include "Player.h"

using namespace DirectX::SimpleMath;

HookPoint::HookPoint(GameObject& gameObject):
	Script(gameObject), state(HookState::NOT_ACTIVE), rb(nullptr), player(nullptr), speed(150.0f), shotTimer(0.0f),
	returnOffset(2.1f, -1.5f, 2.0f)
{
}

HookPoint::~HookPoint()
{
}

void HookPoint::setPlayer(Player* player)
{
	this->player = player;
}

void HookPoint::shoot(const DirectX::SimpleMath::Vector3& orig, const DirectX::SimpleMath::Vector3& vec)
{
	this->rb->setPosition(orig);
	this->rb->setVelocity(vec * this->speed);
	this->shotTimer = 0.7f;
	this->state = HookState::SHOOTING;
}

void HookPoint::returnToPlayer()
{
	this->state = HookState::RETURNING;
}

void HookPoint::setState(HookState newState)
{
	this->state = newState;
}

void HookPoint::init()
{
	this->rb = this->getObject().getComponent<Rigidbody>();
}

void HookPoint::update()
{
	if (this->shotTimer < 0.0f && this->state == HookState::SHOOTING)
		this->state = HookState::RETURNING;
	else if (this->state == HookState::SHOOTING)
		this->shotTimer -= Time::getDT();
	else if (this->state == HookState::RETURNING)
	{
		Transform* playerTransform = this->player->getObject().getComponent<Transform>();
		Vector3 vec = (this->player->getObject().getComponent<Transform>()->getPosition() + 
			this->returnOffset.x * playerTransform->right() +
			this->returnOffset.y * playerTransform->up() +
			this->returnOffset.z * playerTransform->forward()) - 
			this->getTransform()->getPosition();
		if (vec.LengthSquared() < 25.0f)
		{
			this->state = HookState::NOT_ACTIVE;
			this->rb->setVelocity(Vector3::Zero);
		}
		else 
		{
			vec.Normalize();
			this->rb->setVelocity(vec * this->speed);
		}
	}
	else if (this->state == HookState::NOT_ACTIVE)
	{
		Transform* playerTransform = this->player->getObject().getComponent<Transform>();

		this->rb->setPosition(playerTransform->getPosition() + 
			this->returnOffset.x * playerTransform->right() +
			this->returnOffset.y * playerTransform->up() +
			this->returnOffset.z * playerTransform->forward()
		);
	}
}

void HookPoint::onCollisionEnter(GameObject& other)
{
	if (this->state == HookState::SHOOTING && other.getTag() != ObjectTag::PLAYER)
	{
		this->state = HookState::CONNECTED;
		this->rb->setVelocity(Vector3::Zero);

		this->getObject().playSound("HookShootConnect");
	}
}
