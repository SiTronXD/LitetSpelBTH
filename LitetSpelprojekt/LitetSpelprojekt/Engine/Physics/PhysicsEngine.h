#pragma once

#include "../ECS.h"

class PhysicsEngine
{
private:
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void updateCollisions(ECS& ecs);
	//bool raycast(ECS& ecs, DirectX::SimpleMath::Ray ray, GameObject* hitObject, int& distance);
};

