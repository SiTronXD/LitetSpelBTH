#include "Player.h"
#include "../../Engine/Application/Input.h"
#include "../../Engine/GameObject.h"
#include "../../Engine/Time.h"
#include "../../Engine/SMath.h"
#include "../../Engine/Physics/PhysicsEngine.h"
#include "HookPoint.h"
#include "GrapplingHook.h"
#include "CooldownIndicator.h"
#include "Key.h"
#include "Hand.h"

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
		if (vec.LengthSquared() < 9.0f)
			this->hookPoint->returnToPlayer();
		else
		{
			vec.Normalize();
			moveVec *= 0.25f;
			this->rb->addForce(vec * 6.0f * this->speed * Time::getDT() + moveVec);
		}
	}
	else if (this->onGround)
	{
		moveVec.y = this->rb->getVelocity().y;
		this->rb->setVelocity(moveVec);
	}
	else if (moveVec != Vector3::Zero)
	{
		this->rb->addForce(0.5f * moveVec);

		Vector3 vel = this->rb->getVelocity();
		vel.y = 0.0f;
		float velLength = vel.Length();
		if(velLength != 0.0f)
			vel /= velLength; // Normalise
		moveVec.Normalize();

		if (vel.Dot(moveVec) <= -0.25f)
		{
			moveVec *= velLength;
			this->rb->addVelocity(moveVec);
		}
		else
		{
			moveVec *= velLength;
			moveVec.y = this->rb->getVelocity().y;
			this->rb->setVelocity(moveVec);
		}
	}
}

void Player::jump()
{
	if(Input::isKeyJustPressed(Keys::SPACE) && this->onGround)
	{
		this->rb->addVelocity({ 0.0f, this->jumpForce, 0.0f });
		this->onGround = false;

		this->getObject().playSound("Jump");
	}
}

void Player::fireWeapon()
{
	Vector3 forward = this->getTransform()->forward();
	if (Input::isMouseButtonJustPressed(Mouse::LEFT_BUTTON) && this->hookPoint->getState() == HookState::NOT_ACTIVE)
	{
		this->hookPoint->shoot(this->getTransform()->getPosition() + forward * 2.0f, forward);
		this->getObject().playSound("HookShoot");
	}
	else if (Input::isMouseButtonReleased(Mouse::LEFT_BUTTON) && this->hookPoint->getState() != HookState::NOT_ACTIVE)
	{
		this->hookPoint->returnToPlayer();
	}

	if (Input::isMouseButtonDown(Mouse::RIGHT_BUTTON) && this->pulseCannonCooldown <= 0.0f)
	{
		this->rb->addVelocity(forward * -20.0f);
		this->pulseCannonCooldown = this->maxPulseCannonCooldown;

		this->getObject().playSound("PulseCannon");
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

void Player::updateSkyboxColor()
{
	// Should skybox color be updated?
	if (this->lastKeyPieces != this->keyPieces)
	{
		// Update timer
		this->skyboxColorFadeTimer += Time::getDT();

		// Update last key pieces
		if (this->skyboxColorFadeTimer >= this->MAX_SKYBOX_COLOR_FADE_TIME)
			this->lastKeyPieces = this->keyPieces;

		// Clamp timer
		this->skyboxColorFadeTimer = SMath::clamp(
			this->skyboxColorFadeTimer,
			0.0f,
			this->MAX_SKYBOX_COLOR_FADE_TIME
		);

		// Find colors to lerp between
		Vector3 lastColor = this->skyboxColors[this->lastKeyPieces];
		Vector3 nextColor = this->skyboxColors[this->keyPieces];

		// Set color
		this->light->updateColor(
			Vector3::Lerp(
				lastColor,
				nextColor,
				this->skyboxColorFadeTimer / this->MAX_SKYBOX_COLOR_FADE_TIME
			)
		);

		// Reset timer
		if (this->lastKeyPieces == this->keyPieces)
			this->skyboxColorFadeTimer = 0.0f;
	}
}

Player::Player(GameObject& object) :
	Script(object), speed(1000.0f), jumpForce(20.0f), mouseSensitivity(0.5f), maxVelocity(35.0f),
	onGround(false), rb(nullptr),keyPickup(false), keyPieces(0), 
	lastKeyPieces(0), health(3), dead(false), portal(false),
	healthCooldown(0.0f), pulseCannonCooldown(0.0f), maxPulseCannonCooldown(2.5f),
	skyboxColorFadeTimer(0.0f),
	hookPoint(nullptr), grapplingHook(nullptr), 
	cooldownIndicatior(nullptr), light(nullptr), 
	startPosition(0.0f, 0.0f, 0.0f)
{
	Input::setCursorVisible(false);
	Input::setLockCursorPosition(true);

	this->skyboxColors.push_back(Vector3(1.0f,	1.0f, 1.0f)); // No keys
	this->skyboxColors.push_back(Vector3(1.0f,	0.9f, 0.9f)); // 1 key
	this->skyboxColors.push_back(Vector3(0.95f, 0.7f, 0.7f)); // 2 keys
	this->skyboxColors.push_back(Vector3(0.95f, 0.5f, 0.5f)); // 3 keys
	this->skyboxColors.push_back(Vector3(0.9f,	0.3f, 0.3f)); // All keys
}

Player::~Player()
{
}

void Player::setStartPosition(DirectX::SimpleMath::Vector3 pos)
{
	this->startPosition = pos;
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

void Player::setupPointers(
	HookPoint* hp, GrapplingHook* grapHook, 
	CooldownIndicator* cooldown, Light* light,
	Hand* hand)
{
	this->hookPoint = hp;
	this->hookPoint->setPlayer(this);
	this->grapplingHook = grapHook;
	this->grapplingHook->setPlayerTransform(this->getTransform());
	this->cooldownIndicatior = cooldown;
	this->cooldownIndicatior->setup(this->grapplingHook);
	this->light = light;
	this->hand = hand;
}

void Player::setCollectedKeyColor(Vector3 color)
{
	this->collectedKeyColors.push_back(color);
}

std::vector<Vector3> Player::getCollectedKeyColor() const
{
	return this->collectedKeyColors;
}

void Player::takeDamage(float damage)
{
	if (this->healthCooldown <= 0.0f)
	{
		this->health--;
		this->healthCooldown = 0.5f;
	}
}

void Player::resetPlayer(DirectX::SimpleMath::Vector3 pos)
{
	this->rb->setPosition(pos);
	this->hookPoint->setState(HookState::NOT_ACTIVE);
	this->takeDamage(1.0f);
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

	updateSkyboxColor();

	//Add force down
	this->rb->addForce(Vector3(0.0f, -18.0f, 0.0f) * Time::getDT() * 20.0f);

	if (this->rb->getVelocity().LengthSquared() > this->maxVelocity * this->maxVelocity)
	{
		Vector3 vel = this->rb->getVelocity();
		vel.Normalize();
		vel *= this->maxVelocity;
		this->rb->setVelocity(vel);

	}

	if (this->hookPoint->getState() != HookState::NOT_ACTIVE)
		this->grapplingHook->getRope()->setTargetPos(this->hookPoint->getTransform()->getPosition());

	this->cooldownIndicatior->setPercent(1.0f - this->pulseCannonCooldown / this->maxPulseCannonCooldown);
	
	// Update pulse cannon cooldown
	if (this->pulseCannonCooldown > 0.0f)
		this->pulseCannonCooldown -= Time::getDT();

	// Reset keypickup
	if (this->keyPickup)
		this->keyPickup = false;

	// Reset portal
	if (this->portal)
		this->portal = false;

	// Check if player is dead
	if (this->health <= 0)
		this->dead = true;

	// Update health cooldown
	if (this->healthCooldown > 0.0f)
		this->healthCooldown -= Time::getDT();

	if (Input::isKeyJustPressed(Keys::E))
		this->hand->playAnim();
}

void Player::onCollisionEnter(GameObject& other)
{
	// Ground collision
	if (other.getTag() == ObjectTag::GROUND)
	{
		RaycastInfo info = this->getObject().raycast(this->getTransform()->getPosition(), this->getTransform()->getPosition() + Vector3(0.0f, -2.0f, 0.0f));
		if (info.hit)
			this->onGround = true;
	}

	// Collided with key
	if (other.getTag() == ObjectTag::KEY)
	{
		// Remove key
		this->collectedKeyColors.push_back(other.getComponent<Key>()->getKeyColor());
		other.getComponent<Key>()->remove();

		// Play hand animation
		this->hand->playAnim();

		this->keyPieces++;
		this->keyPickup = true;
	}
	// Test
	else if (other.getTag() == ObjectTag::ENEMY && healthCooldown <= 0.0f)
	{
		other.removeComponent<MeshComp>();
		other.removeComponent<Rigidbody>();
		this->takeDamage(1.0f);
	}
	// Portal
	else if (other.getTag() == ObjectTag::PORTAL)
		this->portal = true;
}

void Player::onCollisionStay(GameObject& other)
{
	// Ground collision
	if (other.getTag() == ObjectTag::GROUND)
	{
		RaycastInfo info = this->getObject().raycast(this->getTransform()->getPosition(), this->getTransform()->getPosition() + Vector3(0.0f, -2.0f, 0.0f));
		if (info.hit)
			this->onGround = true;
	}
}

void Player::onCollisionExit(GameObject& other)
{
	if (other.getTag() == ObjectTag::GROUND)
		this->onGround = false;
}
