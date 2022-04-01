#include <iostream>

#include "ECS.h"
#include "GameObject.h"

ECS::ECS()
{
	
}

ECS::~ECS()
{
	// Delete all active components
	for (std::unordered_map<std::type_index, std::vector<Component*>>::iterator itr = this->activeComponents.begin(), 
		itr_end = this->activeComponents.end(); 
		itr != itr_end; ++itr)
	{
		for (Component* component : itr->second)
		{
			delete component;
		}
	}
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

bool ECS::hasComponent(GameObject& gameObject)
{
	// return componentPointers[gameObjectIndex][componentIndex];

	return true;
}

