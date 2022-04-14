#pragma once

#include "Component.h"
#include "Componentpch.h"
#include "../Physics/BoundingVolumes.h"

class Collider : public Component
{
private:
	BoundingVolume* volume;
	bool trigger;

public:
	Collider(GameObject& object);
	virtual ~Collider();

	inline void setCollider(DirectX::BoundingBox box) { delete this->volume; this->volume = new BoxVolume(box); }
	inline void setCollider(DirectX::BoundingOrientedBox orientedBox) { delete this->volume; this->volume = new OrientedBoxVolume(orientedBox); }
	inline void setCollider(DirectX::BoundingSphere sphere) { delete this->volume; this->volume = new SphereVolume(sphere); }

	inline void setTrigger(bool isTrigger) { this->trigger = isTrigger; }
	inline const bool isTrigger() const { return this->trigger; }
	bool isIntersecting(Collider& col) const;
};

