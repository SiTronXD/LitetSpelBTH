#pragma once

#include "Component.h"

class Light : public Component
{
private:
public:
	Light(GameObject& object);
	virtual ~Light();
};

