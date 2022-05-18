#pragma once

#include "../../Engine/Components/Script.h"
#include "../../Engine/Components/MeshComp.h"

class Hand : public Script
{
private:
	const float MAX_PLAY_TIME = 1.5f;

	Transform* transform;
	Transform* targetTransform;

	MeshComp* meshComp;

	float playTimer;

public:
	Hand(GameObject& gameObject);
	virtual ~Hand();

	void setup(GameObject& targetObject);
	void playAnim();

	virtual void init() override;
	virtual void update() override;
};