#pragma once

#include "../../Engine/Components/Script.h"
#include "../../Engine/Components/Componentpch.h"

class GrapplingHook;

class GrapplingHookRope : public Script
{
private:
	GrapplingHook* grapplingHook;
	Transform* transform;

	DirectX::SimpleMath::Vector3 targetPos;
	bool active;

public:
	GrapplingHookRope(GameObject& gameObject);
	virtual ~GrapplingHookRope();

	void setGrapplingHook(GrapplingHook* grapplingHook);
	void setTargetPos(DirectX::SimpleMath::Vector3 target);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};