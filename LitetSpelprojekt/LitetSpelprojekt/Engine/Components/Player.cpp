#include "Player.h"
#include "../Application/Input.h"

void Player::move()
{
	if (Input::isKeyDown(Keys::W))
	{
		//Move Backwards
	}
	else if (Input::isKeyDown(Keys::S))
	{
		//Move Forwards
	}

	if (Input::isKeyDown(Keys::A))
	{
		//Move Left
	}
	else if (Input::isKeyDown(Keys::D))
	{
		//Move Right
	}
	
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

Player::Player()
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
