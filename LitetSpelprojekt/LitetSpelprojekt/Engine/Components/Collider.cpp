#include "Collider.h"
#include "../GameObject.h"

Collider::Collider(GameObject& object):
	Component(object), volume(new BoxVolume(DirectX::BoundingBox())), trigger(false)
{
}

Collider::~Collider()
{
	delete this->volume;
}

bool Collider::isIntersecting(Collider& col) const
{
	return this->volume->intersects(col.volume);
}
