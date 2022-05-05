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
				if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart)
					tempScript->onCollisionEnter(*g2);
				else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit)
					tempScript->onCollisionExit(*g2);
				else
					tempScript->onCollisionStay(*g2);
			}
		}
		for (auto& c : g2->getAllComponents())
		{
			tempScript = dynamic_cast<Script*>(c);
			if (tempScript)
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

void PhysicsEventListener::onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData)
{
	for (unsigned int i = 0; i < callbackData.getNbOverlappingPairs(); i++)
	{
		rp3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(i);
		GameObject* g1 = nullptr;
		GameObject* g2 = nullptr;
		Script* tempScript = nullptr;

		for (auto& rb : this->rbs)
		{
			if (rb->getID() == overlapPair.getBody1()->getEntity().id)
				g1 = &rb->getObject();

			else if (rb->getID() == overlapPair.getBody2()->getEntity().id)
				g2 = &rb->getObject();
		}
		// g1 or g2 are nullptr
		if (!g1 || !g2)
			continue;

		for (auto& c : g1->getAllComponents())
		{
			if (!overlapPair.getCollider1()->getIsTrigger())
				break;

			tempScript = dynamic_cast<Script*>(c);
			if (tempScript)
			{
				if (overlapPair.getEventType() == rp3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
					tempScript->onTriggerEnter(*g2);
				else if (overlapPair.getEventType() == rp3d::OverlapCallback::OverlapPair::EventType::OverlapExit)
					tempScript->onTriggerExit(*g2);
				else
					tempScript->onTriggerStay(*g2);
			}
		}
		for (auto& c : g2->getAllComponents())
		{
			if (!overlapPair.getCollider2()->getIsTrigger())
				break;

			tempScript = dynamic_cast<Script*>(c);
			if (tempScript)
			{
				if (overlapPair.getEventType() == rp3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
					tempScript->onTriggerEnter(*g1);
				else if (overlapPair.getEventType() == rp3d::OverlapCallback::OverlapPair::EventType::OverlapExit)
					tempScript->onTriggerExit(*g1);
				else
					tempScript->onTriggerStay(*g1);
			}
		}
	}
}
