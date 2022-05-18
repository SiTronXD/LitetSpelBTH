#pragma once

#include "../../Engine/Components/Componentpch.h"

class Rigidbody;
class HookPoint;
class GrapplingHook;
class CooldownIndicator;

class Player : public Script
{
private:
	const float MAX_SKYBOX_COLOR_FADE_TIME = 2.0f;

	std::vector<DirectX::SimpleMath::Vector3> skyboxColors;

	DirectX::SimpleMath::Vector3 startPosition;
	std::vector<DirectX::SimpleMath::Vector3> collectedKeyColors;

	float speed;
	float jumpForce;
	float mouseSensitivity;
	float maxVelocity;

	int keyPieces;
	int lastKeyPieces;
	int health;

	float maxPulseCannonCooldown;
	float pulseCannonCooldown;
	float healthCooldown;
	float skyboxColorFadeTimer;

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
	Light* light;
	
	void move();
	void jump();
	void fireWeapon();
	void lookAround();

	void updateSkyboxColor();

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
	void setupPointers(
		HookPoint* hp, GrapplingHook* grapHook, 
		CooldownIndicator* cooldown, Light* light
	);
	
	void setCollectedKeyColor(DirectX::SimpleMath::Vector3 color);
	std::vector<DirectX::SimpleMath::Vector3> getCollectedKeyColor() const;
	

	inline bool isOnGround() const { return this->onGround; }
	inline bool isKeyPickUp() const { return this->keyPickup; }
	inline int getCurrentKeys() const { return this->keyPieces; }
	inline int getHealth() const { return this->health; }

	inline bool isPlayerDead() const { return this->dead; }
	inline bool onPortal() const { return this->portal; }

	void takeDamage(float damage);
	void resetPlayer(DirectX::SimpleMath::Vector3 pos);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};