#pragma once

#include "Component.h"
#include "Componentpch.h"

class BoundingVolume;

class Collider : public Component
{
private:
	BoundingVolume* volume;
	bool trigger;

public:
	Collider(GameObject& object);
	virtual ~Collider();

	void setBoxCollider(DirectX::SimpleMath::Vector3 extents);
	void setOrientedBoxCollider(DirectX::SimpleMath::Vector3 extents);
	void setSphereCollider(float radius);

	inline void setTrigger(bool isTrigger) { this->trigger = isTrigger; }
	inline const bool isTrigger() const { return this->trigger; }
	bool isIntersecting(Collider& col) const;
	bool isIntersecting(DirectX::SimpleMath::Ray ray, float& distance);
};

