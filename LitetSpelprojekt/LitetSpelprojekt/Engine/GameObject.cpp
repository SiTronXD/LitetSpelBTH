#include "GameObject.h"
#include "ECS.h"

GameObject::GameObject(ECS& ecs)
	: ecs(ecs)
{
}

GameObject::~GameObject()
{
}
