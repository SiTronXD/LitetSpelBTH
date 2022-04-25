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
	if(Input::isKeyDown(Keys::SPACE) && this->onGround)
	{
		this->rb->addForce({ 0.0f, this->jumpForce, 0.0f });
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
	Script(object), speed(1000.0f), jumpForce(10.0f), mouseSensitivity(0.5f), onGround(false), rb(nullptr)
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

void Player::onCollisionEnter(GameObject& other)
{
	std::cout << "Player started hitting: " << other.getName() << std::endl;
}

void Player::onCollisionStay(GameObject& other)
{
	std::cout << "Player still hitting: " << other.getName() << std::endl;

	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = true;
}

void Player::onCollisionExit(GameObject& other)
{
	std::cout << "Player stopped hitting: " << other.getName() << std::endl;
}
