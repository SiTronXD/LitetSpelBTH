#include "PhysicsEngine.h"
#include "../GameObject.h"

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::updateCollisions(Scene& scene)
{
	std::vector<Collider*> colVec = scene.getECS().getActiveComponents<Collider>();
	Script* tempScript;

	for (size_t i = 0; i < colVec.size(); i++)
	{
		for (size_t j = i + 1; j < colVec.size(); j++)
		{
			if (colVec[i]->isIntersecting(*colVec[j]))
			{
				GameObject& g1 = colVec[i]->getObject();
				GameObject& g2 = colVec[j]->getObject();
				bool lastCollision = this->lastCollisionStatus.count(std::pair<int, int>(g1.getID(), g2.getID())) > 0;

				for (auto& c : g1.getAllComponents())
				{
					tempScript = dynamic_cast<Script*>(c);
					if (tempScript)
					{
						if (colVec[i]->isTrigger())
						{
							if(lastCollision)
								tempScript->onTriggerStay(g2);
							else
								tempScript->onTriggerEnter(g2);
						}
						else
						{
							if (lastCollision)
								tempScript->onCollisionStay(g2);
							else
								tempScript->onCollisionEnter(g2);
						}
					}
				}
				for (auto& c : g2.getAllComponents())
				{
					tempScript = dynamic_cast<Script*>(c);
					if (tempScript)
					{
						if (colVec[j]->isTrigger())
						{
							if (lastCollision)
								tempScript->onTriggerStay(g1);
							else
								tempScript->onTriggerEnter(g1);
						}
						else
						{
							if (lastCollision)
								tempScript->onCollisionStay(g1);
							else
								tempScript->onCollisionEnter(g1);
						}
					}
				}

				if (!lastCollision)
				{
					this->lastCollisionStatus.insert(
						std::pair<std::pair<int, int>, int>(
							std::pair<int, int>(g1.getID(), g2.getID()),
							true
						)
					);
				}
				else
				{
					this->lastCollisionStatus[std::pair<int, int>(g1.getID(), g2.getID())] = true;
				}
			}
		}
	}

	// Update map, activate exit functions when needed
	auto it = this->lastCollisionStatus.begin();
	while (it != this->lastCollisionStatus.end())
	{
		if (!it->second)
		{
			GameObject& g1 = scene.getECS().getGameObject(it->first.first);
			GameObject& g2 = scene.getECS().getGameObject(it->first.second);

			for (auto& c : g1.getAllComponents())
			{
				tempScript = dynamic_cast<Script*>(c);
				if (tempScript)
				{
					if (g1.getComponent<Collider>()->isTrigger())
						tempScript->onTriggerExit(g2);
					else
						tempScript->onCollisionExit(g2);
				}
			}

			for (auto& c : g2.getAllComponents())
			{
				tempScript = dynamic_cast<Script*>(c);
				if (tempScript)
				{
					if (g2.getComponent<Collider>()->isTrigger())
						tempScript->onTriggerExit(g1);
					else
						tempScript->onCollisionExit(g1);
				}
			}

			it = this->lastCollisionStatus.erase(it);
		}
		else
		{
			it++->second = false;
		}
	}
}

bool PhysicsEngine::raycast(ECS& ecs, DirectX::SimpleMath::Ray ray, GameObject*& hitObject, float& distance)
{
	std::vector<Collider*> colVec = ecs.getActiveComponents<Collider>();
	hitObject = nullptr;
	distance = INFINITY;
	float dist = 0;

	for (auto& col : colVec)
	{
		if (col->isIntersecting(ray, dist) && dist < distance && dist > 0.0f)
		{
			hitObject = &col->getObject();
			distance = dist;
		}
	}
	if (distance < 0.0f) // Non valid hit
		return false;

	return hitObject;
}
