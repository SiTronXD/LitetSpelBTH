#pragma once

#include "Component.h"
#include "Componentpch.h"

class ParticleEmitter : public Component
{
private:
public:
	ParticleEmitter(GameObject& object);
	virtual ~ParticleEmitter();
};

