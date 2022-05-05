#pragma once

#include "Component.h"
#include "Componentpch.h"
#include "../Graphics/ParticleSystem.h"

class ParticleEmitter : public Component
{
private:
	ParticleSystem system;
public:
	ParticleEmitter(GameObject& object);
	virtual ~ParticleEmitter();

	void init(Renderer& renderer, Resources& resources, int nrOfParticles);
	void render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition);
	void explode(float speed, float lifetime);

	inline ParticleSystem& getParticleSystem() { return this->system; }
};

