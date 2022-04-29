#include "PhysicsEventListener.h"
#include "../GameObject.h"
#include "../Components/Rigidbody.h"
#include "../Dev/Log.h"

PhysicsEventListener::PhysicsEventListener()
{
}

PhysicsEventListener::~PhysicsEventListener()
{
}

void PhysicsEventListener::updateRigidbodyVector(std::vector<Rigidbody*> rbs)
{
	this->rbs = rbs;
}

void PhysicsEventListener::onContact(const rp3d::CollisionCallback::CallbackData& callbackData)
{
	for (unsigned int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		rp3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(i);
		GameObject* g1 = nullptr;
		GameObject* g2 = nullptr;
		Script* tempScript = nullptr;

		for (auto& rb : this->rbs)
		{
			if (rb->getID() == contactPair.getBody1()->getEntity().id)
				g1 = &rb->getObject();

			else if (rb->getID() == contactPair.getBody2()->getEntity().id)
				g2 = &rb->getObject();
		}
		// g1 or g2 are nullptr
		if (!g1 || !g2)
			continue;

		for (auto& c : g1->getAllComponents())
		{
			tempScript = dynamic_cast<Script*>(c);
			if (tempScript)
			{
				if (contactPair.getCollider1()->getIsTrigger())
				{
					if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart)
						tempScript->onTriggerEnter(*g2);
					else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit)
						tempScript->onTriggerExit(*g2);
					else
						tempScript->onTriggerStay(*g2);
				}
				else
				{
					if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart)
						tempScript->onCollisionEnter(*g2);
					else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit)
						tempScript->onCollisionExit(*g2);
					else
						tempScript->onCollisionStay(*g2);
				}
			}
		}
		for (auto& c : g2->getAllComponents())
		{
			tempScript = dynamic_cast<Script*>(c);
			if (tempScript)
			{
				if (contactPair.getCollider1()->getIsTrigger())
				{
					if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart)
						tempScript->onTriggerEnter(*g1);
					else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit)
						tempScript->onTriggerExit(*g1);
					else
						tempScript->onTriggerStay(*g1);
				}
				else
				{
					if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart)
						tempScript->onCollisionEnter(*g1);
					else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit)
						tempScript->onCollisionExit(*g1);
					else
						tempScript->onCollisionStay(*g1);
				}
			}
		}
	}
}
