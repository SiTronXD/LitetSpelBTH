#include "Player.h"
#include "../Application/Input.h"
#include "../GameObject.h"

void Player::move()
{
	DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	
	if (Input::isKeyDown(Keys::W))
	{
		direction.z = this->speed;
		
	}
	else if (Input::isKeyDown(Keys::S))
	{
		direction.z = this->speed * -1;
	}

	if (Input::isKeyDown(Keys::A))
	{
		direction.x = this->speed * -1;
	}
	else if (Input::isKeyDown(Keys::D))
	{
		direction.x = this->speed;
	}

	//direction.Normalize();
	this->trans->moveLocal(direction);	
}

void Player::jump()
{

	if(Input::isKeyDown(Keys::SPACE)) //Gonna change it to spacebar later
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
	//Move the camera around
}

Player::Player(GameObject& object):
	Script(object), speed(3.0f)
{
}

Player::~Player()
{
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
