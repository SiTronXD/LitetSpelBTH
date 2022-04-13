#pragma once

#include "Component.h"

class Script : public Component
{
private:
public:
	Script(GameObject& object);
	virtual ~Script();

	virtual void init() = 0;
	virtual void update() = 0;
};

