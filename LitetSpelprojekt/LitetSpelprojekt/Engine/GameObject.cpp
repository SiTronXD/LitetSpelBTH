#include "GameObject.h"
#include "ECS.h"
#include "Physics/PhysicsEngine.h"
#include "AudioEngine.h"

using namespace DirectX::SimpleMath;

GameObject::GameObject(ECS& ecs, int ID, std::string name, ObjectTag tag)
	: ecs(ecs), ID(ID), name(name), tag(tag)
{
}

GameObject::~GameObject()
{
}

RaycastInfo GameObject::raycast(float maxDist)
{
	Transform* transform = this->getComponent<Transform>();
	Vector3 pos = transform->getPosition();
	Vector3 forward = transform->forward();

	rp3d::Vector3 orig = { pos.x, pos.y, pos.z };
	rp3d::Vector3 dir = { forward.x * maxDist, forward.y * maxDist, forward.z * maxDist };

	return this->ecs.getPhysics()->raycast(rp3d::Ray(orig, orig + dir));
}

RaycastInfo GameObject::raycast(DirectX::SimpleMath::Vector3 orig, DirectX::SimpleMath::Vector3 dest)
{
	rp3d::Vector3 rOrig = { orig.x, orig.y, orig.z };
	rp3d::Vector3 rDest = { dest.x, dest.y, dest.z };
	return this->ecs.getPhysics()->raycast(rp3d::Ray(rOrig, rDest));
}

void GameObject::playSound(std::string sound)
{
	this->ecs.getAudio()->playSound(sound);
}
