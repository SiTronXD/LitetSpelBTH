#include "Light.h"
#include "../GameObject.h"

Light::Light(GameObject& object)
	: Component(object), 
	type(LightType::DIRECTIONAL)
{
}

Light::~Light()
{
}
