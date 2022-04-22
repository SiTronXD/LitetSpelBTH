#include "BoundingVolumes.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const Transform* BoundingVolume::getTransform() const
{
	return this->transform;
}

BoundingVolume::BoundingVolume(Transform* transform):
	transform(transform)
{

}

BoundingVolume::~BoundingVolume()
{

}

BoxVolume::BoxVolume(Transform* transform, Vector3 extents):
	BoundingVolume(transform), extents(extents)
{
}

BoxVolume::~BoxVolume()
{
}

bool BoxVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return BoundingBox(this->getTransform()->getPosition(), this->extents).Intersects(vol);
}

bool BoxVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return BoundingBox(this->getTransform()->getPosition(), this->extents).Intersects(vol);
}

bool BoxVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return BoundingBox(this->getTransform()->getPosition(), this->extents).Intersects(vol);
}

bool BoxVolume::intersects(BoundingVolume* vol) const
{
	return vol->intersectsBox(BoundingBox(this->getTransform()->getPosition(), this->extents));
}

bool BoxVolume::intersects(DirectX::SimpleMath::Ray ray, float& distance) const
{
	return BoundingBox(this->getTransform()->getPosition(), this->extents).Intersects(ray.position, ray.direction, distance);
}

DirectX::BoundingOrientedBox OrientedBoxVolume::createOrientedBox() const
{
	Vector3 pos = this->getTransform()->getPosition();
	Vector4 rot = this->getTransform()->getRotationQuat();
	return BoundingOrientedBox(
		pos, 
		this->extents,
		rot
	);
}

OrientedBoxVolume::OrientedBoxVolume(
	Transform* transform, 
	Vector3 extents):
	BoundingVolume(transform), extents(extents)
{
}

OrientedBoxVolume::~OrientedBoxVolume()
{
}

bool OrientedBoxVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return this->createOrientedBox().Intersects(vol);
}

bool OrientedBoxVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return this->createOrientedBox().Intersects(vol);
}

bool OrientedBoxVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return this->createOrientedBox().Intersects(vol);
}

bool OrientedBoxVolume::intersects(BoundingVolume* vol) const
{
	return vol->intersectsOrientedBox(this->createOrientedBox());
}

bool OrientedBoxVolume::intersects(DirectX::SimpleMath::Ray ray, float& distance) const
{
	return this->createOrientedBox().Intersects(ray.position, ray.direction, distance);
}

SphereVolume::SphereVolume(Transform* transform, float radius):
	BoundingVolume(transform), radius(radius)
{
}

SphereVolume::~SphereVolume()
{
}

bool SphereVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return BoundingSphere(this->getTransform()->getPosition(), this->radius).Intersects(vol);
}

bool SphereVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return BoundingSphere(this->getTransform()->getPosition(), this->radius).Intersects(vol);
}

bool SphereVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return BoundingSphere(this->getTransform()->getPosition(), this->radius).Intersects(vol);
}

bool SphereVolume::intersects(BoundingVolume* vol) const
{
	return vol->intersectsSphere(BoundingSphere(this->getTransform()->getPosition(), this->radius));
}

bool SphereVolume::intersects(DirectX::SimpleMath::Ray ray, float& distance) const
{
	return BoundingSphere(this->getTransform()->getPosition(), this->radius).Intersects(ray.position, ray.direction, distance);
}
