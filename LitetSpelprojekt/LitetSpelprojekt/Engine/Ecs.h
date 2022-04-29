#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "Components/Componentpch.h"

#include "Dev/Log.h"

class GameObject;

class ECS
{
private:
	struct pair_hash
	{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	struct ComponentInfo
	{
		Component* component;
		std::type_index componentType;
		int gameObjectID;
	};

	std::vector<GameObject*> gameObjects;
	std::vector<Script*> scriptComps;

	// Map for constant lookups
	// key: GameObject ID and Component type, value: Component*
	std::unordered_map<std::pair<int, std::type_index>, Component*, pair_hash> components;

	// Active components
	// key: Component type, value: Component*
	std::unordered_map<std::type_index, std::vector<Component*>> activeComponents;

	// Components to be removed at the end of the frame
	std::vector<ComponentInfo> componentsToRemove;

	void removeComponents();

	template <typename T>
	bool hasComponent(int gameObjectID);
public:
	ECS();
	~ECS();

	// Initialize Script Components
	void init();
	void update();

	GameObject& addGameObject(std::string name, ObjectTag tag);
	GameObject& getGameObject(int gameObjectID);

	template <typename T>
	T* addComponent(int gameObjectID);
	template <typename T>
	bool removeComponent(int gameObjectID);
	template <typename T>
	T* getComponent(int gameObjectID);

	template <typename T>
	std::vector<T*> getActiveComponents();
	std::vector<Component*> getAllComponents(int gameObjectID);
};

template<typename T>
inline bool ECS::hasComponent(int gameObjectID)
{
	// Check if ID is valid
	if (this->gameObjects.size() <= gameObjectID)
		return false;

	return this->components.count(std::pair<int, std::type_index>(gameObjectID, typeid(T))) > 0;
}

template<typename T>
inline T* ECS::addComponent(int gameObjectID)
{
	// Already have this component
	if (this->hasComponent<T>(gameObjectID))
		return nullptr;

	// Add new vector for component type
	if (this->activeComponents.count(typeid(T)) <= 0)
	{
		this->activeComponents.insert(
			std::pair<std::type_index, std::vector<Component*>>(
				typeid(T),
				std::vector<Component*>()
				)
		);
	}

	// Add new active component
	T* newComponent = new T(*this->gameObjects[gameObjectID]);
	this->activeComponents[typeid(T)].push_back(newComponent);

	Script* sComp = dynamic_cast<Script*>(newComponent);
	if (sComp)
		this->scriptComps.push_back(sComp);

	this->components.insert(
		std::pair<std::pair<int, std::type_index>, Component*>(
				std::pair<int, std::type_index>(gameObjectID, typeid(T)),
				newComponent
		)
	);

	return newComponent;
}

template<typename T>
inline bool ECS::removeComponent(int gameObjectID)
{
	// Don't have this component or type is Transform (not removeable)
	if (!this->hasComponent<T>(gameObjectID) || typeid(T) == typeid(Transform))
		return false;

	Component* comp = this->components[std::pair<int, std::type_index>(gameObjectID, typeid(T))];
	
	// Push component info
	ComponentInfo compInfo
	{
		comp,
		typeid(T),
		gameObjectID
	};
	this->componentsToRemove.push_back(compInfo);

	return true;
}

template<typename T>
inline T* ECS::getComponent(int gameObjectID)
{
	// Don't have this component
	if (!this->hasComponent<T>(gameObjectID))
		return nullptr;

	return dynamic_cast<T*>(this->components[std::pair<int, std::type_index>(gameObjectID, typeid(T))]);
}

template<typename T>
inline std::vector<T*> ECS::getActiveComponents()
{
	// None exist
	if (this->activeComponents.count(typeid(T)) <= 0)
		return std::vector<T*>();

	std::vector<T*> vec;
	for (auto& comp : this->activeComponents[typeid(T)])
		vec.push_back(dynamic_cast<T*>(comp));

	return vec;
}
