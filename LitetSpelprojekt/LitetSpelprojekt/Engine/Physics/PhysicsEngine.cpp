#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::updateCollisions(ECS& ecs)
{
	std::vector<Collider*> colVec = ecs.getActiveComponents<Collider>();

	for (size_t i = 0; i < colVec.size(); i++)
	{
		for (size_t j = i + 1; j < colVec.size(); j++)
		{
			if (colVec[i]->isIntersecting(*colVec[j]))
			{

			}
		}
	}
}

//bool PhysicsEngine::raycast(ECS& ecs, DirectX::SimpleMath::Ray ray, GameObject* hitObject, int& distance)
//{
//	std::vector<Collider*> colVec = ecs.getActiveComponents<Collider>();
//	int dist = INFINITY;
//
//	for (size_t i = 0; i < colVec.size(); i++)
//	{
//		if(/*ray.Intersects(collider, dist) &&*/ dist < distance)
//	}
//
//	return false;
//}
