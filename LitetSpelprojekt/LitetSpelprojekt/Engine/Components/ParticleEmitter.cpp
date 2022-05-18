#include "ParticleEmitter.h"
#include "../GameObject.h"

ParticleEmitter::ParticleEmitter(GameObject& object):
	Component(object), system()
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::init(Renderer& renderer, Resources& resources, int nrOfParticles, DirectX::SimpleMath::Vector3 color1, DirectX::SimpleMath::Vector3 color2)
{
	this->system.setColor(color1, color2);
	this->system.init(renderer, resources, nrOfParticles);
}

void ParticleEmitter::render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition)
{
	this->system.render(vp, cameraPosition);
}

void ParticleEmitter::explode(float speed, float lifetime)
{
	DirectX::SimpleMath::Vector3 position = this->getTransform()->getPosition();
	
	this->system.explode(position, speed, lifetime);
}

void ParticleEmitter::loopable(float speed, float lifetime)
{
	DirectX::SimpleMath::Vector3 position = this->getTransform()->getPosition();

	this->system.loopable(position, speed, lifetime);
}

void ParticleEmitter::stop()
{
	this->system.stop();
}
