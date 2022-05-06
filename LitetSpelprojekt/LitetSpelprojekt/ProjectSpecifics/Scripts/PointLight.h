#pragma once

//#include "../../Engine/Components/Componentpch.h"
#include "../../Engine/Components/Script.h"

class PointLight : public Script
{
private:
	Transform* transform;
	Transform* targetTransform;

public:
	PointLight(GameObject& gameObject);
	virtual ~PointLight();

	void setTarget(GameObject& targetObject);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};