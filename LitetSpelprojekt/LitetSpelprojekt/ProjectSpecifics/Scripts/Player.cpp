#include "Player.h"
#include "../../Engine/Application/Input.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"

using namespace DirectX::SimpleMath;

void Player::move()
{
	
	/*Vector3 direction(0.0f, 0.0f, 0.0f);
	direction = (Input::isKeyDown(Keys::W) - Input::isKeyDown(Keys::S)) * this->FORWARD;
	direction += (Input::isKeyDown(Keys::A) - Input::isKeyDown(Keys::D)) * this->LEFT;*/

	Vector3 direction(Input::isKeyDown(Keys::D) - Input::isKeyDown(Keys::A), 0.0f, Input::isKeyDown(Keys::W) - Input::isKeyDown(Keys::S));
	direction.Normalize();

	Vector3 right = this->getTransform()->right();
	right.y = 0.0f;
	right.Normalize();
	Vector3 forward = this->getTransform()->forward();
	forward.y = 0.0f;
	forward.Normalize();
	this->getTransform()->move((right * direction.x + forward * direction.z) * this->speed * Time::getDT());
}

void Player::jump()
{
	if(Input::isKeyDown(Keys::SPACE)/* && this->onGround*/) //Gonna change it to spacebar later
	{
		this->rb->addForce({ 0.0f, 1.0f, 0.0f });
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
	Script(object), speed(3.0f), mouseSensitive(0.5f), onGround(false), rb(nullptr)
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

	/*GameObject* g = nullptr;
	float distance = 0.0f;
	if (this->getObject().raycast(g, distance))
		std::cout << "Hit Object: " << g->getName() << " Tag: " << (int)g->getTag() << " with distance of: " << distance << std::endl;*/
}

void Player::onCollisionStay(GameObject& other)
{
	std::cout << "Player hit: " << other.getName() << std::endl;
}
