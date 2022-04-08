#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Light.h"
#include "Components/Mesh.h"
#include "Components/ParticleEmitter.h"
#include "Components/Rigidbody.h"
#include "Components/Collider.h"
#include "Components/Script.h"

#include "Dev/Log.h"

//class GameObject;

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

	std::vector<GameObject> gameObjects;
	std::vector<Script*> scriptComps;

	// Map for constant lookups
	// key: GameObject ID and Component type, value: Component*
	std::unordered_map<std::pair<int, std::type_index>, Component*, pair_hash> components;

	// Active components
	// key: Component type, value: Component*
	std::unordered_map<std::type_index, std::vector<Component*>> activeComponents;

	template <typename T>
	bool hasComponent(GameObject& gameObject);
public:
	ECS();
	~ECS();

	void update();

	GameObject& addGameObject();

	template <typename T>
	T* addComponent(GameObject& gameObject);
	template <typename T>
	bool removeComponent(GameObject& gameObject);
	template <typename T>
	T* getComponent(GameObject& gameObject);
};

template<typename T>
inline bool ECS::hasComponent(GameObject& gameObject)
{
	return this->components.count(std::pair<int, std::type_index>(gameObject.getID(), typeid(T))) > 0;
}

template<typename T>
inline T* ECS::addComponent(GameObject& gameObject)
{
	// Already have this component
	if (this->hasComponent<T>(gameObject))
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
	T* newComponent = new T();
	this->activeComponents[typeid(T)].push_back(newComponent);

	Script* sComp = dynamic_cast<Script*>(newComponent);
	if (sComp)
		this->scriptComps.push_back(sComp);

	this->components.insert(
		std::pair<std::pair<int, std::type_index>, Component*>(
				std::pair<int, std::type_index>(gameObject.getID(), typeid(T)),
				newComponent
		)
	);

	return newComponent;
}

template<typename T>
inline bool ECS::removeComponent(GameObject& gameObject)
{
	// Don't have this component
	if (!this->hasComponent<T>(gameObject))
		return false;

	Component* comp = this->components[std::pair<int, std::type_index>(gameObject.getID(), typeid(T))];
	this->components[std::pair<int, std::type_index>(gameObject.getID(), typeid(T))] = nullptr;

	for (size_t i = 0; i < this->activeComponents[typeid(T)].size(); i++)
	{
		if (comp == this->activeComponents[typeid(T)][i])
		{
			this->activeComponents[typeid(T)].erase(this->activeComponents[typeid(T)].begin() + i);
			break;
		}
	}

	delete comp;
	return true;
}

template<typename T>
inline T* ECS::getComponent(GameObject& gameObject)
{
	// Don't have this component
	if (!this->hasComponent<T>(gameObject))
		return nullptr;

	return dynamic_cast<T*>(this->components[std::pair<int, std::type_index>(gameObject.getID(), typeid(T))]);
}
