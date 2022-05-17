#pragma once

#include "../../Engine/Components/Script.h"


class Key : public Script
{
private:
	GameObject* pointLight;
	DirectX::SimpleMath::Vector3 keyColor;
=======
	GameObject* beam;
  
public:
	Key(GameObject& gameObject);
	virtual ~Key();

	void setPointLight(GameObject* pointLight);
	void setKeyColor(DirectX::SimpleMath::Vector3 color);
	
	DirectX::SimpleMath::Vector3 getKeyColor() const;

	void set(GameObject* pointLight, GameObject* beam);

	void remove();

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onCollisionStay(GameObject& other) override;
	virtual void onCollisionExit(GameObject& other) override;


};