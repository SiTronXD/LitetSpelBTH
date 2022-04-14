#include "Player.h"
#include "../Application/Input.h"
#include "../GameObject.h"
#include "../Time.h"

void Player::move()
{
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	
	if (Input::isKeyDown(Keys::W))
	{
		direction = this->FORWARD;
	}
	else if (Input::isKeyDown(Keys::S))
	{
		direction = this->BACKWARD;
	}

	if (Input::isKeyDown(Keys::A))
	{
		direction = this->LEFT;
	}
	else if (Input::isKeyDown(Keys::D))
	{
		direction = this->RIGHT;
	}

	direction.Normalize();
	this->getTransform()->moveLocal(direction * this->speed * Time::getDT());
	
}

void Player::jump()
{

	if(Input::isKeyDown(Keys::SPACE) && this->onGround) //Gonna change it to spacebar later
	{
		//Jump
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
	DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	int cursorDeltaX = Input::getCursorDeltaX();
	
	if (cursorDeltaX != 0)
	{
		rotation = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	}

	rotation.Normalize();
	this->getTransform()->rotate(rotation * this->mouseSensitiv);
}

Player::Player(GameObject& object):
	Script(object), speed(3.0f), mouseSensitiv(3.0f), onGround(false)
{
}

Player::~Player()
{
}

void Player::setSpeed(float spd)
{
	this->speed = spd;
}

void Player::init()
{
}

void Player::update()
{
	move();
	jump();
	fireWeapon();
	lookAround();
}
