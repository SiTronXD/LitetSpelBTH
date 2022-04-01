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

bool ECS::hasComponent(GameObject& gameObject)
{
	// return componentPointers[gameObjectIndex][componentIndex];

	return true;
}
