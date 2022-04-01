#include "GameObject.h"
#include "ECS.h"

GameObject::GameObject(ECS& ecs)
	: ecs(ecs)
{
	// All game objects should have a transform
	this->addComponent<Transform>();
}

GameObject::~GameObject()
{
}