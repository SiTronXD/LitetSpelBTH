#include "Script.h"

Script::Script(GameObject& object):
	Component(object)
{
}

Script::~Script()
{
}

void Script::onCollisionStay(GameObject& other)
{ }

void Script::onTriggerStay(GameObject& other)
{ }