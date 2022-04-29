#include "Script.h"

Script::Script(GameObject& object):
	Component(object)
{
}

Script::~Script()
{
}

void Script::onCollisionEnter(GameObject& other)
{
}

void Script::onCollisionStay(GameObject& other)
{ 
}

void Script::onCollisionExit(GameObject & other)
{
}

void Script::onTriggerEnter(GameObject& other)
{
}

void Script::onTriggerStay(GameObject& other)
{
}

void Script::onTriggerExit(GameObject & other)
{
}
