#include <iostream>

#include "ECS.h"
#include "GameObject.h"

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
	for (GameObject* g : this->gameObjects)
		delete g;
}

void ECS::init()
{
	for (auto& sComp : this->scriptComps)
	{
		sComp->init();
	}
}

void ECS::update()
{
	for (auto& sComp : this->scriptComps)
	{
		sComp->update();
	}
}

GameObject& ECS::addGameObject(std::string name, ObjectTag tag = ObjectTag::UNTAGGED)
{
	int ID = this->gameObjects.size();

	// All game objects should have a transform
	this->gameObjects.push_back(new GameObject(*this, ID, name, tag));
	this->addComponent<Transform>(ID);

	return *this->gameObjects.back();
}

GameObject& ECS::getGameObject(int gameObjectID)
{
	return this->getComponent<Transform>(gameObjectID)->getObject();
}

std::vector<Component*> ECS::getAllComponents(int gameObjectID)
{
	std::vector<Component*> comps;
	std::vector<std::type_index> keys;
	// Get keys
	for (std::unordered_map<std::type_index, std::vector<Component*>>::iterator itr = this->activeComponents.begin(),
		itr_end = this->activeComponents.end();
		itr != itr_end; ++itr)
	{
		keys.push_back(itr->first);
	}

	// Get Components via the keys
	for (auto& key : keys)
	{
		if (this->components.count(std::pair<int, std::type_index>(gameObjectID, key)) <= 0)
			continue;

		comps.push_back(this->components[std::pair<int, std::type_index>(gameObjectID, key)]);
	}

	return comps;
}

