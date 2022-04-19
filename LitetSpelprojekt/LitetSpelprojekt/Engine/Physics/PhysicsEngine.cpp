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
				for (auto& c : g1.getAllComponents())
				{
					tempScript = dynamic_cast<Script*>(c);
					if (tempScript)
					{
						if (colVec[i]->isTrigger())
							tempScript->onTriggerStay(g2);
						else
							tempScript->onCollisionStay(g2);
					}
				}
				for (auto& c : g2.getAllComponents())
				{
					tempScript = dynamic_cast<Script*>(c);
					if (tempScript)
					{
						if (colVec[j]->isTrigger())
							tempScript->onTriggerStay(g1);
						else
							tempScript->onCollisionStay(g1);
					}
				}
			}
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
