#include "GameObject.h"
#include "ECS.h"
#include "Physics/PhysicsEngine.h"

using namespace DirectX::SimpleMath;

GameObject::GameObject(ECS& ecs, int ID, std::string name, ObjectTag tag)
	: ecs(ecs), ID(ID), name(name), tag(tag)
{
}

GameObject::~GameObject()
{
}
