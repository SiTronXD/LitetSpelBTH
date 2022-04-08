#include <iostream>

#include "ECS.h"
//#include "GameObject.h"

ECS::ECS()
{
	// Add new vector for script component type
	if (this->activeComponents.count(typeid(Script)) <= 0)
	{
		this->activeComponents.insert(
			std::pair<std::type_index, std::vector<Component*>>(
				typeid(Script),
				std::vector<Component*>()
				)
		);
	}
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
	for (auto& sComp : this->scriptComps)
	{
		sComp->update();
	}
}

GameObject& ECS::addGameObject()
{
	this->gameObjects.push_back(GameObject(*this, this->gameObjects.size()));
	return this->gameObjects.back();
}

//bool ECS::hasComponent(GameObject& gameObject)
//{
//	// return componentPointers[gameObjectIndex][componentIndex];
//
//	return true;
//}

