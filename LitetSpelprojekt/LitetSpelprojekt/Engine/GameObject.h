#pragma once

#include "ECS.h"

class GameObject
{
private:
	ECS& ecs;
	int ID;
public:
	GameObject(ECS& ecs, int ID);
	~GameObject();

	int getID() const;

	template <typename T>
	T* addComponent();
	template <typename T>
	bool removeComponent();
	template <typename T>
	T* getComponent();
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
