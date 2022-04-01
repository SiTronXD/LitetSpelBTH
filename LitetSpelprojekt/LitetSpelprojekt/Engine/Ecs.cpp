#include <iostream>

#include "ECS.h"

ECS::ECS()
{
	
}

ECS::~ECS()
{
}

void ECS::update()
{
	/*
	
	for(Transform t : activeTransformComponents)
	{
		t.update();
	}
	
	*/
}

void ECS::addGameObject(GameObject& gameObjectToAdd)
{
	this->gameObjects.push_back(gameObjectToAdd);
}

template <typename T>
void ECS::addActiveComponent(GameObject& gameObject)
{
	// Add new vector for component type
	if (this->activeComponents.count(T) <= 0)
	{
		this->activeComponents.insert(
			std::pair<std::type_index, std::vector<Component*>>(
				typeid(Transform),
				std::vector<Component*>()
				)
		);
	}

	this->activeComponents[T].push_back(T);
}

bool ECS::hasComponent(GameObject& gameObject)
{
	// return componentPointers[gameObjectIndex][componentIndex];

	return true;
}

