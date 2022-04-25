#include "Collider.h"
#include "../GameObject.h"
#include "../Physics/BoundingVolumes.h"

using namespace DirectX::SimpleMath;

Collider::Collider(GameObject& object):
	Component(object), trigger(false)
{
	this->setBoxCollider(Vector3(1, 1, 1));
}

Collider::~Collider()
{
	delete this->volume;
}

void Collider::setBoxCollider(DirectX::SimpleMath::Vector3 extents)
{
	delete this->volume;
	this->volume = new BoxVolume(this->getTransform(), extents);
}

void Collider::setOrientedBoxCollider(
	DirectX::SimpleMath::Vector3 extents)
{
	delete this->volume;
	this->volume = new OrientedBoxVolume(
		this->getTransform(), 
		extents
	);
}

void Collider::setSphereCollider(float radius)
{
	delete this->volume;
	this->volume = new SphereVolume(this->getTransform(), radius);
}

bool Collider::isIntersecting(Collider& col) const
{
	return this->volume->intersects(col.volume);
}

bool Collider::isIntersecting(DirectX::SimpleMath::Ray ray, float& distance)
{
	return this->volume->intersects(ray, distance);
}
