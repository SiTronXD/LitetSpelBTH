#pragma once

#include "GrapplingHook.h"
#include "../../Engine/Components/MeshComp.h"

class Hand : public Script
{
private:
	const float MAX_PLAY_TIME = 1.5f;

	Transform* transform;
	Transform* targetTransform;

	GrapplingHook* grapplingHook;

	MeshComp* meshComp;

	float playTimer;

	float getPositionOffset();

public:
	Hand(GameObject& gameObject);
	virtual ~Hand();

	void setup(GameObject& grapplingHookObject);
	void playAnim();

	virtual void init() override;
	virtual void update() override;
};