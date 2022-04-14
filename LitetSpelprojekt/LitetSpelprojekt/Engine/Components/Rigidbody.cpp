#include "Rigidbody.h"
#include "../GameObject.h"

Rigidbody::Rigidbody(GameObject& object):
	Component(object)
{
}

Rigidbody::~Rigidbody()
{
}
