#include "GameObject.h"
#include "ECS.h"

GameObject::GameObject(ECS& ecs, int ID)
	: ecs(ecs), ID(ID)
{
}

GameObject::~GameObject()
{
}
