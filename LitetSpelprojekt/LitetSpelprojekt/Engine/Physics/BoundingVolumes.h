#pragma once
#include <Windows.h>
#include <SimpleMath.h>
#include "../Components/Transform.h"

class BoundingVolume
{
private:
	Transform* transform;
protected:
	const Transform* getTransform() const;
public:
	BoundingVolume(Transform* transform);
	virtual ~BoundingVolume();

	virtual bool intersectsBox(DirectX::BoundingBox vol) const = 0;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const = 0;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const = 0;

	virtual bool intersects(BoundingVolume* vol) const = 0;
	virtual bool intersects(DirectX::SimpleMath::Ray ray, float& distance) const = 0;
};

class BoxVolume : public BoundingVolume
{
private:
	DirectX::SimpleMath::Vector3 extents;
public:
	BoxVolume(Transform* transform, DirectX::SimpleMath::Vector3 extents);
	virtual ~BoxVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	// Inherited via BoundingVolume
	virtual bool intersects(BoundingVolume* vol) const override;
	virtual bool intersects(DirectX::SimpleMath::Ray ray, float& distance) const override;
};

class OrientedBoxVolume : public BoundingVolume
{
private:
	DirectX::SimpleMath::Vector3 extents;

	DirectX::BoundingOrientedBox createOrientedBox() const;
public:
	OrientedBoxVolume(
		Transform* transform, 
		DirectX::SimpleMath::Vector3 extents);
	virtual ~OrientedBoxVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	// Inherited via BoundingVolume
	virtual bool intersects(BoundingVolume* vol) const override;
	virtual bool intersects(DirectX::SimpleMath::Ray ray, float& distance) const override;
};

class SphereVolume : public BoundingVolume
{
private:
	float radius;
public:
	SphereVolume(Transform* transform, float radius);
	virtual ~SphereVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	// Inherited via BoundingVolume
	virtual bool intersects(BoundingVolume* vol) const override;
	virtual bool intersects(DirectX::SimpleMath::Ray ray, float& distance) const override;
};


