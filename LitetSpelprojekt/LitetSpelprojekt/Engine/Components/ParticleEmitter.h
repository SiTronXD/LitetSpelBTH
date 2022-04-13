#pragma once

#include "Component.h"

class ParticleEmitter : public Component
{
private:
public:
	ParticleEmitter(GameObject& object);
	virtual ~ParticleEmitter();
};

