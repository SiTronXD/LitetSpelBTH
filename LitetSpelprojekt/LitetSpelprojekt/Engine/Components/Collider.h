#pragma once

#include "Component.h"

class Collider : public Component
{
private:
public:
	Collider(GameObject& object);
	virtual ~Collider();
};

