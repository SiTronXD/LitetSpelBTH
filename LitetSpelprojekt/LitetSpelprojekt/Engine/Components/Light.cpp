#include "Light.h"
#include "../GameObject.h"

Light::Light(GameObject& object):
	Component(object)
{
}

Light::~Light()
{
}
