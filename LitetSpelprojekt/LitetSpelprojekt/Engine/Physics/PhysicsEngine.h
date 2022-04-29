#pragma once

#include "../ECS.h"
#include "../Application/Scene.h"

class PhysicsEngine
{
private:
	struct pair_hash
	{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	// key: ID of two intersecting objects, value: bool to check if collision is still active during update
	std::unordered_map<std::pair<int, int>, bool, pair_hash> lastCollisionStatus;
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void updateCollisions(Scene& scene);
	static bool raycast(ECS& ecs, DirectX::SimpleMath::Ray ray, GameObject*& hitObject, float& distance);
};

