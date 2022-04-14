#include "BoundingVolumes.h"

BoundingVolume::BoundingVolume()
{

}

BoundingVolume::~BoundingVolume()
{

}

BoxVolume::BoxVolume(DirectX::BoundingBox box):
	box(box)
{
}

BoxVolume::~BoxVolume()
{
}

bool BoxVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return this->box.Intersects(vol);
}

bool BoxVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return this->box.Intersects(vol);
}

bool BoxVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return this->box.Intersects(vol);
}

bool BoxVolume::intersects(BoundingVolume* vol) const
{
	return this->intersectsBox(this->box);
}

OrientedBoxVolume::OrientedBoxVolume(DirectX::BoundingOrientedBox orientBox):
	orientBox(orientBox)
{
}

OrientedBoxVolume::~OrientedBoxVolume()
{
}

bool OrientedBoxVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return this->orientBox.Intersects(vol);
}

bool OrientedBoxVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return this->orientBox.Intersects(vol);
}

bool OrientedBoxVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return this->orientBox.Intersects(vol);
}

bool OrientedBoxVolume::intersects(BoundingVolume* vol) const
{
	return this->intersectsOrientedBox(this->orientBox);
}

SphereVolume::SphereVolume(DirectX::BoundingSphere sphere):
	sphere(sphere)
{
}

SphereVolume::~SphereVolume()
{
}

bool SphereVolume::intersectsBox(DirectX::BoundingBox vol) const
{
	return this->sphere.Intersects(vol);
}

bool SphereVolume::intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const
{
	return this->sphere.Intersects(vol);
}

bool SphereVolume::intersectsSphere(DirectX::BoundingSphere vol) const
{
	return this->sphere.Intersects(vol);
}

bool SphereVolume::intersects(BoundingVolume* vol) const
{
	return this->intersectsSphere(this->sphere);
}
