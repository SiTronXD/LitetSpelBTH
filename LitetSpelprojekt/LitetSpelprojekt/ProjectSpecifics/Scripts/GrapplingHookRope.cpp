#include "GrapplingHookRope.h"
#include "../../Engine/Time.h"
#include "../../Engine/GameObject.h"

using namespace DirectX::SimpleMath;

GrapplingHookRope::GrapplingHookRope(GameObject& gameObject)
	: Script(gameObject),
	grapplingHook(nullptr),
	transform(nullptr)
{
}

GrapplingHookRope::~GrapplingHookRope()
{
}

void GrapplingHookRope::setGrapplingHook(GrapplingHook* grapplingHook)
{
	this->grapplingHook = grapplingHook;
}

void GrapplingHookRope::init()
{
	this->transform = Script::getObject().getComponent<Transform>();
	this->targetPos = Vector3(0,0,0);
}

void GrapplingHookRope::update()
{
	// Activate when grappling hook shoots
	if (false)
	{
		// Update position
		Vector3 newPos;
		this->grapplingHook->setToShootPos(newPos);
		this->transform->setPosition(newPos);

		Vector3 dir = this->targetPos - newPos;

		// Update rotation
		this->transform->setRotation(dir, Vector3(0, 1, 0));

		// Update scale
		this->transform->setScaling(0.1f, 0.1f, dir.Length());
	}
	else
	{
		// Update scale
		this->transform->setScaling(0.0f, 0.0f, 0.0f);
	}
}

void GrapplingHookRope::onCollisionEnter(GameObject& other)
{
}

void GrapplingHookRope::onCollisionStay(GameObject& other)
{
}

void GrapplingHookRope::onCollisionExit(GameObject& other)
{
}
