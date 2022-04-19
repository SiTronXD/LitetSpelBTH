#pragma once

#include <Windows.h>
#include <SimpleMath.h>
#include "Component.h"
#include "Componentpch.h"

class Rigidbody : public Script
{
private:
	DirectX::SimpleMath::Vector3 acceleration;
	DirectX::SimpleMath::Vector3 velocity;

	Transform& transform;

	float mass;

	bool isKinematic;

public:
	Rigidbody(GameObject& object);
	virtual ~Rigidbody();

	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionStay(GameObject& other) override;

	void setMass(float newMass);
	void setVelocity(const DirectX::SimpleMath::Vector3& newVelocity);
	void addForce(const DirectX::SimpleMath::Vector3& newForce);
};

