#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "../GameObject.h"

struct RaycastInfo
{
	bool hit;
	GameObject* gameObject;
	float distance;
	float hitfraction;
	DirectX::SimpleMath::Vector3 hitPoint;
	DirectX::SimpleMath::Vector3 hitNormal;

	RaycastInfo() :
		hit(false), gameObject(nullptr), distance(0.0f), hitfraction(0.0f), hitPoint(DirectX::SimpleMath::Vector3::Zero), hitNormal(DirectX::SimpleMath::Vector3::Zero)
	{ }
};

class PhysicsRaycastCallback : public rp3d::RaycastCallback
{
private:
	RaycastInfo& rayInfo;
	std::vector<Rigidbody*> rbs;
public:
	PhysicsRaycastCallback(RaycastInfo& info, std::vector<Rigidbody*> rbs);
	virtual ~PhysicsRaycastCallback();

	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info);
};

