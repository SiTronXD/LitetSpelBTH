#pragma once

#include "Component.h"
#include "Componentpch.h"

class Rigidbody : public Component
{
private:
public:
	Rigidbody(GameObject& object);
	virtual ~Rigidbody();
};

