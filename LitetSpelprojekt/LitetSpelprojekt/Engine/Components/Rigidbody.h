#pragma once

#include "Component.h"

class Rigidbody : public Component
{
private:
public:
	Rigidbody(GameObject& object);
	virtual ~Rigidbody();
};

