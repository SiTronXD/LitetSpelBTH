#pragma once

#include "ECS.h"

class GameObject
{
private:
	ECS& ecs;

public:
	GameObject(ECS& ecs);
	~GameObject();

	template <typename T>
	void addComponent();
};

template<typename T>
inline void GameObject::addComponent()
{
	this->ecs.addActiveComponent<T>(*this);
}
