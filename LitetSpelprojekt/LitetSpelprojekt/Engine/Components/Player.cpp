#include "Player.h"
#include "../Application/Input.h"
#include "../GameObject.h"
#include "../Time.h"

void Player::move()
{
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	
	direction = (Input::isKeyDown(Keys::W) - Input::isKeyDown(Keys::S)) * this->FORWARD;

	direction += (Input::isKeyDown(Keys::A) - Input::isKeyDown(Keys::D)) * this->LEFT;

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
	DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3(Input::getCursorDeltaY(), Input::getCursorDeltaX(), 0.0f);
	this->getTransform()->rotate(-rotation * this->mouseSensitive);
}

Player::Player(GameObject& object):
	Script(object), speed(3.0f), mouseSensitive(0.5f), onGround(false)
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
