#pragma once

#include "../../Engine/Components/Script.h"

class GrapplingHook : public Script
{
private:
	Transform* thisTransform;
	Transform* playerTransform;

public:
	GrapplingHook(GameObject& gameObject);
	virtual ~GrapplingHook();

	void setPlayerTransform(Transform* playerTransform);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};

