#pragma once

#include "ECS.h"

class GameObject
{
private:
	ECS& ecs;
	int ID;

	std::string name;
	ObjectTag tag;
public:
	GameObject(ECS& ecs, int ID, std::string name, ObjectTag tag);
	~GameObject();

	template <typename T>
	T* addComponent();
	template <typename T>
	bool removeComponent();
	template <typename T>
	T* getComponent();

	inline const int getID() { return this->ID; };
	inline const std::string getName() { return this->name; }
	inline const ObjectTag getTag() { return this->tag; }
	inline std::vector<Component*> getAllComponents() { return this->ecs.getAllComponents(this->ID); };


	// Cast from own position and direction
	const bool raycast(GameObject*& hitObject, float& distance);
	// Define own ray position and direction
	const bool raycast(DirectX::SimpleMath::Ray ray, GameObject*& hitObject, float& distance);
};

template<typename T>
inline T* GameObject::addComponent()
{
	return this->ecs.addComponent<T>(this->ID);
}

template<typename T>
inline bool GameObject::removeComponent()
{
	return this->ecs.removeComponent<T>(this->ID);
}

template<typename T>
inline T* GameObject::getComponent()
{
	return this->ecs.getComponent<T>(this->ID);
}
