#pragma once

#include "GrapplingHook.h"

class GrapplingHookRope : public Script
{
private:
	GrapplingHook* grapplingHook;

	Transform* transform;

	DirectX::SimpleMath::Vector3 targetPos;

public:
	GrapplingHookRope(GameObject& gameObject);
	virtual ~GrapplingHookRope();

	void setGrapplingHook(GrapplingHook* grapplingHook);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};