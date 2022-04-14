#pragma once

#include "Component.h"
#include "Componentpch.h"

class Collider : public Component
{
private:
public:
	Collider(GameObject& object);
	virtual ~Collider();
};

