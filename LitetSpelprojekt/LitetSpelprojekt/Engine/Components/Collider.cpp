#include "Collider.h"
#include "../GameObject.h"

Collider::Collider(GameObject& object):
	Component(object)
{
}

Collider::~Collider()
{
}
