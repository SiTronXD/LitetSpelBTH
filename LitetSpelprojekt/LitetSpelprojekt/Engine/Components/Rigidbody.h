#pragma once

#include <vector>
#include <Windows.h>
#include <SimpleMath.h>
#include <reactphysics3d/reactphysics3d.h>
#include "Component.h"
#include "Componentpch.h"

class PhysicsEngine;

class Rigidbody : public Script
{
private:
	PhysicsEngine* physEngine;
	Transform& transform;
	rp3d::RigidBody* rb;
	std::vector<rp3d::Collider*> colliders;

	DirectX::SimpleMath::Vector3 lastPos;
	DirectX::SimpleMath::Vector3 nextPos;

	rp3d::Transform getConvertedTransform();
	
public:
	Rigidbody(GameObject& object);
	virtual ~Rigidbody();

	void setPhysics(PhysicsEngine& physicsEngine);

	void addBoxCollider(DirectX::SimpleMath::Vector3 extents, DirectX::SimpleMath::Vector3 posOffset = DirectX::SimpleMath::Vector3::Zero);
	void addSphereCollider(float radius, DirectX::SimpleMath::Vector3 posOffset = DirectX::SimpleMath::Vector3::Zero);
	void addCapsuleCollider(float radius, float height, DirectX::SimpleMath::Vector3 posOffset = DirectX::SimpleMath::Vector3::Zero);

	void setMass(float mass);
	void setType(rp3d::BodyType type);
	void setGravity(bool status);
	//void setTrigger(bool status); Never again :)
	void setMaterial(float frictionCoeff, float bounciness, float massDensity = 1.0f);
	void setPosition(DirectX::SimpleMath::Vector3 pos);
	void setPosRestrict(DirectX::SimpleMath::Vector3 restrictVec);
	void setRotRestrict(DirectX::SimpleMath::Vector3 restrictVec);

	DirectX::SimpleMath::Vector3 getVelocity() const;
	void setVelocity(DirectX::SimpleMath::Vector3 vec);
	void addVelocity(DirectX::SimpleMath::Vector3 vec);
	void setAngularVelocity(DirectX::SimpleMath::Vector3 vec);

	void addForce(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Vector3 posOffset = DirectX::SimpleMath::Vector3::Zero);
	void addForceWorldSpace(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Vector3 point = DirectX::SimpleMath::Vector3::Zero);

	void updateStates();

	int getID() const;

	virtual void init() override;
	virtual void update() override;

	inline rp3d::RigidBody* getRB() { return this->rb; }
};

