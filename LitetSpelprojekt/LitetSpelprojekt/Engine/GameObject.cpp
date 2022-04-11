#include "GameObject.h"
#include "ECS.h"

GameObject::GameObject(ECS& ecs, int ID)
	: ecs(ecs), ID(ID)
{
	// All game objects should have a transform
	this->addComponent<Transform>();
}

GameObject::~GameObject()
{
}

int GameObject::getID() const
{
	return this->ID;
}
