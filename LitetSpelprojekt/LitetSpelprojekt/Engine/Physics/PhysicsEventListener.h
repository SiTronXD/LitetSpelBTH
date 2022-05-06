#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

class Rigidbody;

class PhysicsEventListener : public rp3d::EventListener
{
private:
	std::vector<Rigidbody*> rbs;
public:
	PhysicsEventListener();
	virtual ~PhysicsEventListener();

	void updateRigidbodyVector(std::vector<Rigidbody*> rbs);

	virtual void onContact(const rp3d::CollisionCallback::CallbackData& callbackData) override;
	virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData) override;
};

