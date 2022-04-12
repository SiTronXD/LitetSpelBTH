#pragma once
#include "Script.h"
class Player : public Script
{
private:
	void move();
	void jump();
	void fireWeapon();
	void lookAround();
public:
	Player();
	~Player();

	// Inherited via Script
	virtual void init() override;

	virtual void update() override;

};

