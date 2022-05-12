#pragma once

#include "../../Engine/Components/Script.h"

class Key : public Script
{
private:

	GameObject* pointLight;

public:
	Key(GameObject& gameObject);
	virtual ~Key();

	void setPointLight(GameObject* pointLight);

	void remove();

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;
};