#pragma once

#include "Script.h"
#include "Componentpch.h"

class Player : public Script
{
private:
	float speed;

	void move();
	void jump();
	void fireWeapon();
	void lookAround();
public:
	Player(GameObject& object);
	~Player();

	// Inherited via Script
	virtual void init() override;

	virtual void update() override;

};

