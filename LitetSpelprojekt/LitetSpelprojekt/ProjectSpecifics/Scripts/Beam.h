#pragma once

#include <Windows.h>
#include <SimpleMath.h>
#include "../../Engine/Components/Script.h"

class Beam : public Script
{
private:
	const float TARGET_MAX_DIST = 220.0f;

	Transform* transform;
	Transform* targetTransform;

	DirectX::SimpleMath::Vector3 startPos;
	DirectX::SimpleMath::Vector3 startScale;

	float timer;

public:
	Beam(GameObject& gameObject);
	virtual ~Beam();

	void set(GameObject& keyObject, GameObject& targetObject);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
};