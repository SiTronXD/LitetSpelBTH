#include <iostream>

#include "ECS.h"
#include "GameObject.h"

void ECS::removeComponents()
{
	for (int i = this->componentsToRemove.size() - 1; i >= 0; --i)
	{
		// Component information
		ComponentInfo compInfo = this->componentsToRemove[i];
		Component* comp = compInfo.component;
		std::type_index compType = compInfo.componentType;
		int gameObjectID = compInfo.gameObjectID;

		// Erase from components
		this->components.erase(std::pair<int, std::type_index>(gameObjectID, compType));

		// Erase from active components
		for (size_t i = 0; i < this->activeComponents[compType].size(); i++)
		{
			if (comp == this->activeComponents[compType][i])
			{
				this->activeComponents[compType].erase(this->activeComponents[compType].begin() + i);
				break;
			}
		}

		// Erase from script components
		Script* sComp = dynamic_cast<Script*>(comp);
		if (sComp != nullptr)
		{
			for (size_t i = 0; i < this->scriptComps.size(); i++)
			{
				if (sComp == this->scriptComps[i])
				{
					this->scriptComps.erase(this->scriptComps.begin() + i);
					break;
				}
			}
		}

		delete comp;

		// Remove element
		this->componentsToRemove.erase(this->componentsToRemove.begin() + i);
	}
}

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

	this->removeComponents();
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

