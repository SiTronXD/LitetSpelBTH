#include "GrapplingHook.h"
#include "../../Engine/Dev/Log.h"
#include "../../Engine/GameObject.h"

GrapplingHook::GrapplingHook(GameObject& gameObject)
	: Script(gameObject),
	thisTransform(nullptr),
	playerTransform(nullptr)
{
	this->thisTransform = gameObject.getComponent<Transform>();
}

GrapplingHook::~GrapplingHook()
{
}

void GrapplingHook::setPlayerTransform(Transform* playerTransform)
{
	this->playerTransform = playerTransform;
}

void GrapplingHook::init()
{

}

void GrapplingHook::update()
{
}

void GrapplingHook::onCollisionEnter(GameObject& other)
{
}

void GrapplingHook::onCollisionStay(GameObject& other)
{
}

void GrapplingHook::onCollisionExit(GameObject& other)
{
}
