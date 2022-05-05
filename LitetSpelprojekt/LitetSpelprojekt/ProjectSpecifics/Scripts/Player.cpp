#include "Player.h"
#include "../../Engine/Application/Input.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"

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
	moveVec.y = this->rb->getVelocity().y;
	this->rb->setVelocity(moveVec);
}

void Player::jump()
{
	if(Input::isKeyJustPressed(Keys::SPACE) && this->onGround)
	{
		//this->rb->addForce({ 0.0f, this->jumpForce, 0.0f });
		this->rb->addVelocity({ 0.0f, this->jumpForce, 0.0f });
		this->onGround = false;
	}
}

void Player::fireWeapon()
{
	if (Input::isMouseButtonDown(Mouse::LEFT_BUTTON))
	{
		//Shoot grapplinghook
	}

	if (Input::isMouseButtonDown(Mouse::RIGHT_BUTTON))
	{
		//Shoot forcegun
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

Player::Player(GameObject& object):
	Script(object), speed(1000.0f), jumpForce(10.0f), mouseSensitivity(0.5f), onGround(false), rb(nullptr), 
	keyPickup(false), keyPieces(0), health(3), dead(false), healthCooldown(0.0f), portal(false)
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

	//Reset keypickup
	if (this->keyPickup == true)
		this->keyPickup = false;

	//Reset portal
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

	// Remove key mesh and rigidbody if collided by player
	if (other.getTag() == ObjectTag::KEY)
	{
		other.removeComponent<MeshComp>();
		other.removeComponent<Rigidbody>();
		this->keyPieces++;
		this->keyPickup = true;
	}

}

void Player::onCollisionStay(GameObject& other)
{
	//std::cout << "Player still hitting: " << other.getName() << std::endl;

	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = true;
	
	//Test
	if (other.getTag() == ObjectTag::ENEMY && healthCooldown == 0)
	{
		other.removeComponent<MeshComp>();
		other.removeComponent<Rigidbody>();
		this->health--;	
		this->healthCooldown = 40;
	}

	//Portal
	if (other.getTag() == ObjectTag::PORTAL)
		this->portal = true;
	
}

void Player::onCollisionExit(GameObject& other)
{
	//std::cout << "Player stopped hitting: " << other.getName() << std::endl;

	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = false;
}
