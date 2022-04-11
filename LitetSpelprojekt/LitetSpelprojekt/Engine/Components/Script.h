#pragma once

#include "Component.h"

class Script : public Component
{
private:
public:
	virtual void init() = 0;
	virtual void update() = 0;
};

