#pragma once

#include "../../Engine/Components/Componentpch.h"

class Rigidbody;
class HookPoint;
class GrapplingHook;
class CooldownIndicator;

class Player : public Script
{
private:
	DirectX::SimpleMath::Vector3 startPosition;
	
	float speed;
	float jumpForce;
	float mouseSensitivity;
	float maxVelocity;

	int keyPieces;
	int health;

	float maxPulseCannonCooldown;
	float pulseCannonCooldown;
	float healthCooldown;

	bool onGround;
	bool keyPickup;
	bool dead;
	bool portal;

	// Component references
	Rigidbody* rb;

	// References to other gameObject components
	HookPoint* hookPoint;
	GrapplingHook* grapplingHook;
	CooldownIndicator* cooldownIndicatior;
	
	void move();
	void jump();
	void fireWeapon();
	void lookAround();

public:
	Player(GameObject& object);
	~Player();

	void setStartPosition(DirectX::SimpleMath::Vector3 pos);
	inline DirectX::SimpleMath::Vector3 getStartPosition() { return this->startPosition; }

	inline float getSpeed() const { return this->speed; }
	void setSpeed(float speed);
	
	inline float getJumpForce() const { return this->jumpForce; }
	void setJumpForce(float jumpForce);

	inline float getMouseSensitivity() const { return this->mouseSensitivity; }
	void setMouseSensitivity(float mouseSensitivity);
	void setHealth(int health);
	void addHealth(int health);
	void setGrapplingHook(HookPoint* hp, GrapplingHook* grapHook, CooldownIndicator* cooldown);
	
	inline bool isOnGround() const { return this->onGround; }
	inline bool isKeyPickUp() const { return this->keyPickup; }
	inline int getCurrentKeys() const { return this->keyPieces; }
	inline int getHealth() const { return this->health; }
	
	inline bool isPlayerDead() const { return this->dead; }
	inline bool onPortal() const { return this->portal; }

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};

