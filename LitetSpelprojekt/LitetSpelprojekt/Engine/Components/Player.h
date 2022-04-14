#pragma once

#include "Script.h"
#include "Componentpch.h"

class Player : public Script
{
private:
	float speed;
	float mouseSensitiv;

	bool onGround;
	
	void move();
	void jump();
	void fireWeapon();
	void lookAround();

public:
	Player(GameObject& object);
	~Player();

	inline float getSpeed() const { return this->speed; }
	void setSpeed(float spd);

	inline bool isOnGround() const { return this->onGround; }

	// Inherited via Script
	virtual void init() override;

	virtual void update() override;

private:
	const DirectX::SimpleMath::Vector3 FORWARD = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f);
	const DirectX::SimpleMath::Vector3 BACKWARD = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	const DirectX::SimpleMath::Vector3 LEFT = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
	const DirectX::SimpleMath::Vector3 RIGHT = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);

};

