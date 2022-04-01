#pragma once

#include <vector>
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