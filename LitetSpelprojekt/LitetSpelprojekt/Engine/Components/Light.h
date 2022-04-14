#pragma once

#include "Component.h"
#include "Componentpch.h"

class Light : public Component
{
private:
public:
	Light(GameObject& object);
	virtual ~Light();
};

