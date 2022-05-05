#include "Player.h"
#include "../../Engine/Application/Input.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "HookPoint.h"

using namespace DirectX::SimpleMath;

void Player::move()
{
	Vector3 direction((float)(Input::isKeyDown(Keys::D) - Input::isKeyDown(Keys::A)), 0.0f, (float)(Input::isKeyDown(Keys::W) - Input::isKeyDown(Keys::S)));
	direction.Normalize();

	Vector3 right = this->getTransform()->right();
	right.y = 0.0f;
	right.Normalize();
	Vector3 forward = this->getTransform()->forward();
	forward.y = 0.0f;
	forward.Normalize();
	Vector3 moveVec = (right * direction.x + forward * direction.z) * this->speed * Time::getDT();

	if (this->hookPoint->getState() == HookState::CONNECTED)
	{
		Vector3 vec = this->hookPoint->getTransform()->getPosition() - this->getTransform()->getPosition();
		if (vec.Length() < 3.0f)
			this->hookPoint->returnToPlayer(this->hookPointOffset);
		else
		{
			vec.Normalize();
			moveVec *= 0.5f;
			this->rb->setVelocity(vec * this->speed * Time::getDT() + moveVec);
		}
	}
	else
	{
		moveVec.y = this->rb->getVelocity().y;
		this->rb->setVelocity(moveVec);
	}
}

void Player::jump()
{
	if(Input::isKeyJustPressed(Keys::SPACE) && this->onGround)
	{
		this->rb->addVelocity({ 0.0f, this->jumpForce, 0.0f });
		this->onGround = false;
	}
}

void Player::fireWeapon()
{
	Vector3 forward = this->getTransform()->forward();
	if (Input::isMouseButtonJustPressed(Mouse::LEFT_BUTTON) && this->hookPoint->getState() == HookState::NOT_ACTIVE)
	{
		this->hookPoint->shoot(this->getTransform()->getPosition() + forward * 2, forward);
	}
	else if (Input::isMouseButtonReleased(Mouse::LEFT_BUTTON) && this->hookPoint->getState() != HookState::NOT_ACTIVE)
	{
		this->hookPoint->returnToPlayer(this->hookPointOffset);
		//this->hookPoint->shoot(this->hookPoint->getTransform()->getPosition(), -forward);
		//this->hookPoint->getObject().getComponent<Rigidbody>()->setPosition(this->getTransform()->getPosition() + forward * this->hookPointOffset);
	}

	if (Input::isMouseButtonDown(Mouse::RIGHT_BUTTON))
	{
		Log::writeVec(forward);
		this->rb->addForce(forward * -200.0f);
	}
}

void Player::lookAround()
{
	Vector3 rotation((float)Input::getCursorDeltaY(), (float)Input::getCursorDeltaX(), 0.0f);
	this->getTransform()->rotate(-rotation * this->mouseSensitivity);

	Vector3 origRot = this->getTransform()->getRotation();
	if (origRot.x > 80.0f)
		origRot.x = 80.0f;
	else if (origRot.x < -80.0f)
		origRot.x = -80.0f;
	this->getTransform()->setRotation(origRot);
}

Player::Player(GameObject& object) :
	Script(object), speed(1000.0f), jumpForce(10.0f), mouseSensitivity(0.5f), onGround(false), rb(nullptr), hookPoint(nullptr),
	keyPickup(false), keyPieces(0), health(3), dead(false), healthCooldown(0.0f), portal(false), hookPointOffset(Vector3(0.0f, 0.0f, 3.0f))
{
	Input::setCursorVisible(false);
	Input::setLockCursorPosition(true);
}

Player::~Player()
{
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
}

void Player::setJumpForce(float jumpForce)
{
	this->jumpForce = jumpForce;
}

void Player::setMouseSensitivity(float mouseSensitivity)
{
	this->mouseSensitivity = mouseSensitivity;
}

void Player::setHealth(int health)
{
	this->health = health;
}

void Player::addHealth(int health)
{
	this->health += health;
}

void Player::setHookPoint(HookPoint* hp)
{
	this->hookPoint = hp;
	this->hookPoint->setPlayer(this);
}

void Player::init()
{
	this->rb = this->getObject().getComponent<Rigidbody>();
	if (this->rb)
		Log::write("Player found rigidbody");
}

void Player::update()
{
	move();
	jump();
	fireWeapon();
	lookAround();

	/*if (this->hooking && !this->hookPoint->inMotion() && !this->hookPoint->isConnected())
		this->hooking = false;*/

	//Reset keypickup
	if (this->keyPickup == true)
		this->keyPickup = false;

	//Rest portal
	if (this->portal == true)
		this->portal = false;

	//Check if player is dead
	if (this->health < 1)
		this->dead = true;

	//Update health cooldown
	if (this->healthCooldown > 0)
		this->healthCooldown--;
}

void Player::onCollisionEnter(GameObject& other)
{
	//std::cout << "Player started hitting: " << other.getName() << std::endl;
	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = true;
}

void Player::onCollisionStay(GameObject& other)
{
	//std::cout << "Player still hitting: " << other.getName() << std::endl;

	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = true;
	
	if (other.getTag() == ObjectTag::KEY)
	{
		other.removeComponent<MeshComp>();
		//other.removeComponent<Collider>();
		//other.getComponent<Transform>()->setPosition(0.0f, -100.0f, 0.0f);
		this->keyPieces++;
		this->keyPickup = true;
	}

	//Test
	if (other.getTag() == ObjectTag::ENEMY && healthCooldown == 0)
	{
		other.removeComponent<MeshComp>();
		other.getComponent<Transform>()->setPosition(0.0f, -100.0f, 0.0f);
		this->health--;	
		this->healthCooldown = 40;
	}

	//Poral
	if (other.getTag() == ObjectTag::PORTAL)
		this->portal = true;
	
}

void Player::onCollisionExit(GameObject& other)
{
	//std::cout << "Player stopped hitting: " << other.getName() << std::endl;

	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = false;
}
