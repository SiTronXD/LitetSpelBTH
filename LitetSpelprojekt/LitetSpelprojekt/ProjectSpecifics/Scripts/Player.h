#pragma once

#include "../../Engine/Components/Script.h"
#include "../../Engine/Components/Componentpch.h"

class Rigidbody;

class Player : public Script
{
private:
	float speed;
	float jumpForce;
	float mouseSensitivity;

	bool onGround;

	// Component references
	Rigidbody* rb;
	
	void move();
	void jump();
	void fireWeapon();
	void lookAround();

public:
	Player(GameObject& object);
	~Player();

	inline float getSpeed() const { return this->speed; }
	void setSpeed(float speed);
	inline float getJumpForce() const { return this->jumpForce; }
	void setJumpForce(float jumpForce);

	inline bool isOnGround() const { return this->onGround; }

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};

