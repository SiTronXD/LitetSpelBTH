#include "GrapplingHookRope.h"
#include "GrapplingHook.h"
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

void GrapplingHookRope::setTargetPos(DirectX::SimpleMath::Vector3 target)
{
	this->targetPos = target;
	this->active = true;
}

void GrapplingHookRope::init()
{
	this->transform = this->getTransform();
	this->targetPos = Vector3(0,0,0);
}

void GrapplingHookRope::update()
{
	// Activate when grappling hook shoots
	if (this->active)
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
		this->active = false;
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
