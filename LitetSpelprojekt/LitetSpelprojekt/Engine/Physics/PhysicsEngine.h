#pragma once

#include "../ECS.h"
#include "../Application/Scene.h"

class PhysicsEngine
{
private:
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void updateCollisions(Scene& scene);
	static bool raycast(ECS& ecs, DirectX::SimpleMath::Ray ray, GameObject*& hitObject, float& distance);
};

