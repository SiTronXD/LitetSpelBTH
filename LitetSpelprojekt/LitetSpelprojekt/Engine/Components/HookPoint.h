#pragma once
#include "Script.h"
#include "Componentpch.h"

class HookPoint : public Script
{
private:
	Rigidbody* rb;
	DirectX::SimpleMath::Vector3 shootVec;
	float speed;
	bool connected;
	float shotTimer; // Is in motion with a timer until it no longer won't be
public:
	HookPoint(GameObject& gameObject);
	virtual ~HookPoint();

	// True if it connected to some surface
	void shoot(const DirectX::SimpleMath::Vector3& orig, const DirectX::SimpleMath::Vector3& vec);
	bool isConnected() const;
	bool inMotion() const;

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	virtual void onCollisionEnter(GameObject& other) override;
	virtual void onTriggerEnter(GameObject& other) override;
};

