#pragma once

#include "../../Engine/Components/Script.h"

class Key : public Script
{
private:
	GameObject* pointLight;
	GameObject* beam;

public:
	Key(GameObject& gameObject);
	virtual ~Key();

	void set(GameObject* pointLight, GameObject* beam);

	void remove();

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};