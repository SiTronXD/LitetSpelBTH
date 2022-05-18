#pragma once

#include "../../Engine/Components/Script.h"

class Hand : public Script
{
private:
	Transform* transform;
	Transform* targetTransform;

public:
	Hand(GameObject& gameObject);
	virtual ~Hand();

	void setup(GameObject& targetObject);

	virtual void init() override;
	virtual void update() override;
};