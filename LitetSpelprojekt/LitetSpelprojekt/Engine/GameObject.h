#pragma once

#include "ECS.h"

class GameObject
{
private:
	ECS& ecs;
	std::string name;
	std::string tag;


public:
	GameObject(ECS& ecs);
	GameObject(ECS& ecs, std::string name, std::string tag);
	~GameObject();

	void setName(std::string name);
	void setTag(std::string tag);
	std::string getName() const;
	std::string getTag() const;

	template <typename T>
	void addComponent();
};

template<typename T>
inline void GameObject::addComponent()
{
	this->ecs.addActiveComponent<T>(*this);
}
