#include "PhysicsRaycastCallback.h"

PhysicsRaycastCallback::PhysicsRaycastCallback(RaycastInfo& info, std::vector<Rigidbody*> rbs):
	rayInfo(info), rbs(rbs)
{
}

PhysicsRaycastCallback::~PhysicsRaycastCallback()
{
}

rp3d::decimal PhysicsRaycastCallback::notifyRaycastHit(const rp3d::RaycastInfo& info)
{
	this->rayInfo.hit = false;
	for (auto& rb : this->rbs)
	{
		if (rb->getID() == info.body->getEntity().id)
		{
			this->rayInfo.gameObject = &rb->getObject();
			this->rayInfo.hit = true;
			break;
		}
	}
	this->rayInfo.hitfraction = info.hitFraction;
	this->rayInfo.hitPoint = { info.worldPoint.x, info.worldPoint.y, info.worldPoint.z };
	this->rayInfo.hitNormal = { info.worldNormal.x, info.worldNormal.y, info.worldNormal.z };

	return rp3d::decimal(info.hitFraction);
}
