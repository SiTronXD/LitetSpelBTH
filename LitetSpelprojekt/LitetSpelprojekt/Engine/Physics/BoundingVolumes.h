#pragma once
#include <DirectXCollision.h>

class BoundingVolume
{
private:
public:
	BoundingVolume();
	virtual ~BoundingVolume();

	virtual bool intersectsBox(DirectX::BoundingBox vol) const = 0;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const = 0;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const = 0;

	virtual bool intersects(BoundingVolume* vol) const = 0;

};

class BoxVolume : public BoundingVolume
{
private:
	DirectX::BoundingBox box;
public:
	BoxVolume(DirectX::BoundingBox box);
	virtual ~BoxVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	virtual bool intersects(BoundingVolume* vol) const override;
};

class OrientedBoxVolume : public BoundingVolume
{
private:
	DirectX::BoundingOrientedBox orientBox;
public:
	OrientedBoxVolume(DirectX::BoundingOrientedBox orientBox);
	virtual ~OrientedBoxVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	virtual bool intersects(BoundingVolume* vol) const override;

};

class SphereVolume : public BoundingVolume
{
private:
	DirectX::BoundingSphere sphere;
public:
	SphereVolume(DirectX::BoundingSphere sphere);
	virtual ~SphereVolume();

	// Inherited via BoundingVolume
	virtual bool intersectsBox(DirectX::BoundingBox vol) const override;
	virtual bool intersectsOrientedBox(DirectX::BoundingOrientedBox vol) const override;
	virtual bool intersectsSphere(DirectX::BoundingSphere vol) const override;

	virtual bool intersects(BoundingVolume* vol) const override;

};


