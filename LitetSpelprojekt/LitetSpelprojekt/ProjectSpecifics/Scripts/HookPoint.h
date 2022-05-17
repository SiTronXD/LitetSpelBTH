#pragma once

#include "../../Engine/Components/Script.h"
#include "../../Engine/Components/Componentpch.h"

enum class HookState { NOT_ACTIVE, SHOOTING, CONNECTED, RETURNING };

class Player;
class Rigidbody;

class HookPoint : public Script
{
private:
	HookState state;
	//Rigidbody* rb;
	Transform* transform;
	Player* player;
	float speed;
	float shotTimer; // Is in motion with a timer until time runs out

	DirectX::SimpleMath::Vector3 velocity;
	DirectX::SimpleMath::Vector3 returnOffset;
public:
	HookPoint(GameObject& gameObject);
	virtual ~HookPoint();

	void setPlayer(Player* player);

	void shoot(const DirectX::SimpleMath::Vector3& orig, const DirectX::SimpleMath::Vector3& vec);
	void returnToPlayer();

	inline HookState getState() { return this->state; }
	void setState(HookState newState);

	// Inherited via Script
	virtual void init() override;
	virtual void update() override;
	//virtual void onCollisionEnter(GameObject& other) override;
};