#pragma once

#include <vector>
#include "GameObject.h"
#include "Components/Transform.h"

class ECS
{
private:
	std::vector<GameObject> gameObjects;

	Component** componentPointers;

	std::vector<Transform*> activeTransformComponents;
public:
	ECS();
	~ECS();

	void update();

	void addGameObject(GameObject& gameObjectToAdd);

	bool hasComponent(GameObject& gameObject);
};