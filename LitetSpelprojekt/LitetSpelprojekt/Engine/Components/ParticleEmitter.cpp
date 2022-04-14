#include "ParticleEmitter.h"
#include "../GameObject.h"

ParticleEmitter::ParticleEmitter(GameObject& object):
	Component(object)
{
}

ParticleEmitter::~ParticleEmitter()
{
}
