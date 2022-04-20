#include "GameObject.h"
#include "ECS.h"
#include "Physics/PhysicsEngine.h"

GameObject::GameObject(ECS& ecs, int ID, std::string name, ObjectTag tag)
	: ecs(ecs), ID(ID), name(name), tag(tag)
{
}

GameObject::~GameObject()
{
}

const bool GameObject::raycast(GameObject*& hitObject, float& distance)
{
	Transform* transform = this->getComponent<Transform>();
	return PhysicsEngine::raycast(this->ecs, DirectX::SimpleMath::Ray(transform->getPosition(), transform->forward()), hitObject, distance);
}

const bool GameObject::raycast(DirectX::SimpleMath::Ray ray, GameObject*& hitObject, float& distance)
{
	return PhysicsEngine::raycast(this->ecs, ray, hitObject, distance);
}
