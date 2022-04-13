#include "Component.h"
#include "../GameObject.h"

Component::Component(GameObject& object):
	object(object)
{
	this->transform = this->object.getComponent<Transform>();
}

Component::~Component()
{
}
