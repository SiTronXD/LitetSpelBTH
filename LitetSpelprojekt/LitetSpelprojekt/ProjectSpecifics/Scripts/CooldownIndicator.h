#pragma once

#include "GrapplingHook.h"
#include "../../Engine/Components/AbsoluteMeshComp.h"

class CooldownIndicator : public Script
{
private:
	GrapplingHook* grapplingHook;

	Transform* transform;
	AbsoluteMeshComp* mesh;

	DirectX::SimpleMath::Vector3 fullColor;
	DirectX::SimpleMath::Vector3 emptyColor;

	float percent;

public:
	CooldownIndicator(GameObject& gameObject);
	virtual ~CooldownIndicator();

	void setup(GrapplingHook* grapplingHook);
	void setPercent(float percent);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};