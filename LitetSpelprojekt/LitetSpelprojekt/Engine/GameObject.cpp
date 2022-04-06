#include "GameObject.h"
#include "ECS.h"

GameObject::GameObject(ECS& ecs)
	: ecs(ecs)
{
	// All game objects should have a transform
	this->addComponent<Transform>();
}

GameObject::GameObject(ECS& ecs, std::string name, std::string tag)
	:ecs(ecs), name(name), tag(tag)
{
	this->addComponent<Transform>();
}

GameObject::~GameObject()
{
}

void GameObject::setName(std::string name)
{
	this->name = name;
}

void GameObject::setTag(std::string tag)
{
	this->tag = tag;
}

std::string GameObject::getName() const
{
	return this->name;
}

std::string GameObject::getTag() const
{
	return this->tag;
}
