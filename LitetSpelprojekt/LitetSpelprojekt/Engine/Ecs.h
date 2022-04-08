#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Light.h"
#include "Components/MeshComp.h"
#include "Components/ParticleEmitter.h"
#include "Components/Rigidbody.h"
#include "Components/Collider.h"
#include "Components/Script.h"

class GameObject;

class ECS
{
private:
	std::vector<GameObject> gameObjects;

	// Map for constant lookups
	// key: game object and component type, value: component*
	// <insert map here>

	// Active components
	// key: component type, value: component*
	std::unordered_map<std::type_index, std::vector<Component*>> activeComponents;

public:
	ECS();
	~ECS();

	void update();

	void addGameObject(GameObject& gameObjectToAdd);

	template <typename T>
	void addActiveComponent(GameObject& gameObject);

	bool hasComponent(GameObject& gameObject);
};

template<typename T>
inline void ECS::addActiveComponent(GameObject& gameObject)
{
	// Add new vector for component type
	if (this->activeComponents.count(typeid(T)) <= 0)
	{
		this->activeComponents.insert(
			std::pair<std::type_index, std::vector<Component*>>(
				typeid(Transform),
				std::vector<Component*>()
				)
		);
	}

	// Add new active component
	T* newComponent = new T();
	this->activeComponents[typeid(T)].push_back(newComponent);
}
