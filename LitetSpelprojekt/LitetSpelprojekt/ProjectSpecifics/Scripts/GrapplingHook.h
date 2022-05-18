#pragma once

#include <Windows.h>
#include <SimpleMath.h>
#include "../../Engine/Components/Script.h"
#include "GrapplingHookRope.h"

class GrapplingHook : public Script
{
private:
	const float SWAY_FACTOR = 7.0f;
	const float MAX_DIST = 0.6f;

	Transform* transform;
	Transform* playerTransform;
	GrapplingHookRope* rope;

	DirectX::SimpleMath::Vector3 lastPos;
	DirectX::SimpleMath::Vector3 offsetPos;
	DirectX::SimpleMath::Vector3 swayDeltaPos;

	void setToArmPos(DirectX::SimpleMath::Vector3& result);

public:
	GrapplingHook(GameObject& gameObject);
	virtual ~GrapplingHook();

	void setPlayerTransform(Transform* playerTransform);
	void setRope(GrapplingHookRope* rope);
	void setToShootPos(DirectX::SimpleMath::Vector3& result);

	inline GrapplingHookRope* getRope() { return this->rope; }

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;

	inline DirectX::SimpleMath::Vector3 getSwayDeltaPos() { return this->swayDeltaPos; }
};

