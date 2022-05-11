#include "Rigidbody.h"
#include "../Physics/PhysicsEngine.h"
#include "../GameObject.h"
#include "../Time.h"

using namespace DirectX::SimpleMath;

rp3d::Transform Rigidbody::getConvertedTransform()
{
	Vector3 pos = this->transform.getPosition();
	Vector4 quat = this->transform.getRotationQuat();
	if (quat.x == 0.0f &&
		quat.y == 0.0f &&
		quat.z == 0.0f &&
		quat.w == 0.0f)
	{
		quat.w = 1.0f;
	}

	rp3d::Transform transform;
	transform.setPosition(rp3d::Vector3(pos.x, pos.y, pos.z));
	transform.setOrientation(rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w));
	return transform;
}

Rigidbody::Rigidbody(GameObject& object) :
	Script(object), transform(*this->getTransform()),
	physEngine(nullptr), rb(nullptr)
{
	this->lastPos = this->transform.getPosition();
	this->nextPos = this->lastPos;
}

Rigidbody::~Rigidbody()
{
	// 1.
	//this->physEngine->getWorld()->destroyRigidBody(this->rb);
	 
	// 2. 
	//this->rb->enableGravity(false);
	/*for(int i = this->rb->getNbColliders() - 1; i >= 0; --i)
		this->rb->removeCollider(this->rb->getCollider(i));*/

	// 3.
	// TODO: recreate physics world
	this->rb->setIsSleeping(true);
}

void Rigidbody::setPhysics(PhysicsEngine& physicsEngine)
{
	if (this->physEngine != nullptr)
		return;

	this->physEngine = &physicsEngine;
	this->rb = this->physEngine->getWorld()->createRigidBody(this->getConvertedTransform());
	this->rb->setLinearLockAxisFactor(rp3d::Vector3(1.0f, 1.0f, 1.0f));
	this->rb->setType(rp3d::BodyType::DYNAMIC);
	this->rb->setMass(1.0f);
}

void Rigidbody::addBoxCollider(DirectX::SimpleMath::Vector3 extents, DirectX::SimpleMath::Vector3 posOffset)
{
	rp3d::Transform transform = rp3d::Transform::identity();
	transform.setPosition({ posOffset.x, posOffset.y, posOffset.z });

	this->colliders.push_back(this->rb->addCollider(this->physEngine->getCommon().createBoxShape({ extents.x, extents.y, extents.z }), transform));
	this->rb->updateLocalCenterOfMassFromColliders();
	this->colliders.back()->getMaterial().setFrictionCoefficient(0.2f);
	this->colliders.back()->getMaterial().setBounciness(0.0f);
}

void Rigidbody::addSphereCollider(float radius, DirectX::SimpleMath::Vector3 posOffset)
{
	rp3d::Transform transform = rp3d::Transform::identity();
	transform.setPosition({ posOffset.x, posOffset.y, posOffset.z });

	this->colliders.push_back(this->rb->addCollider(this->physEngine->getCommon().createSphereShape(radius), transform));
	this->rb->updateLocalCenterOfMassFromColliders();
	this->colliders.back()->getMaterial().setFrictionCoefficient(0.2f);
	this->colliders.back()->getMaterial().setBounciness(0.0f);
}

void Rigidbody::addCapsuleCollider(float radius, float height, DirectX::SimpleMath::Vector3 posOffset)
{
	rp3d::Transform transform = rp3d::Transform::identity();
	transform.setPosition({ posOffset.x, posOffset.y, posOffset.z });

	this->colliders.push_back(this->rb->addCollider(this->physEngine->getCommon().createCapsuleShape(radius, height), transform));
	this->rb->updateLocalCenterOfMassFromColliders();
	this->colliders.back()->getMaterial().setFrictionCoefficient(0.2f);
	this->colliders.back()->getMaterial().setBounciness(0.0f);
}

void Rigidbody::setMass(float mass)
{
	this->rb->setMass(mass);
}

void Rigidbody::setType(rp3d::BodyType type)
{
	this->rb->setType(type);
}

void Rigidbody::setGravity(bool status)
{
	this->rb->enableGravity(status);
}

void Rigidbody::setTrigger(bool status)
{
	for (auto& col : this->colliders)
	{
		col->setIsTrigger(status);
	}
}

void Rigidbody::setMaterial(float frictionCoeff, float bounciness, float massDensity)
{
	if (!this->colliders.size())
		return;

	rp3d::Material mat = this->colliders[0]->getMaterial();
	mat.setFrictionCoefficient(frictionCoeff);
	mat.setBounciness(bounciness);
	mat.setMassDensity(massDensity);

	for (auto& col : this->colliders)
	{
		col->setMaterial(mat);
	}
}

void Rigidbody::setPosition(DirectX::SimpleMath::Vector3 vec)
{
	rp3d::Transform transform = this->rb->getTransform();
	transform.setPosition({ vec.x, vec.y, vec.z });
	this->rb->setTransform(transform);
}

void Rigidbody::setPosRestrict(DirectX::SimpleMath::Vector3 restrictVec)
{
	this->rb->setLinearLockAxisFactor({ restrictVec.x, restrictVec.y, restrictVec.z });
}

void Rigidbody::setRotRestrict(DirectX::SimpleMath::Vector3 restrictVec)
{
	this->rb->setAngularLockAxisFactor({ restrictVec.x, restrictVec.y, restrictVec.z });
}

DirectX::SimpleMath::Vector3 Rigidbody::getVelocity() const
{
	rp3d::Vector3 vel = this->rb->getLinearVelocity();
	return Vector3(vel.x, vel.y, vel.z);
}

void Rigidbody::setVelocity(DirectX::SimpleMath::Vector3 vec)
{
	this->rb->setLinearVelocity({ vec.x, vec.y, vec.z });
}

void Rigidbody::setAngularVelocity(DirectX::SimpleMath::Vector3 vec)
{
	this->rb->setAngularVelocity({ vec.x, vec.y, vec.z });
}

void Rigidbody::addVelocity(DirectX::SimpleMath::Vector3 vec)
{
	this->rb->setLinearVelocity(this->rb->getLinearVelocity() + rp3d::Vector3(vec.x, vec.y, vec.z));
}

void Rigidbody::addForce(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Vector3 posOffset)
{
	this->rb->applyLocalForceAtLocalPosition({ vec.x, vec.y, vec.z }, { posOffset.x, posOffset.y, posOffset.z });
}

void Rigidbody::addForceWorldSpace(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Vector3 point)
{
	this->rb->applyLocalForceAtWorldPosition({ vec.x, vec.y, vec.z }, { point.x, point.y, point.z });
}

void Rigidbody::updateStates()
{
	if (this->rb->getLinearLockAxisFactor() != rp3d::Vector3::zero())
	{
		rp3d::Vector3 pos = this->rb->getTransform().getPosition();
		this->lastPos = this->nextPos;
		this->nextPos.x = pos.x;
		this->nextPos.y = pos.y;
		this->nextPos.z = pos.z;
	}
}

int Rigidbody::getID() const
{
	return this->rb->getEntity().id;
}

void Rigidbody::init()
{
}

void Rigidbody::update()
{
	if (this->rb->getLinearLockAxisFactor() != rp3d::Vector3::zero())
	{
		// Set to interpolated position
		this->transform.setPosition(
			Vector3::Lerp(
				this->lastPos,
				this->nextPos,
				PhysicsEngine::getInterpolationFactor()
			)
		);
	}

	if (this->rb->getAngularLockAxisFactor() != rp3d::Vector3::zero())
	{
		rp3d::Quaternion quat = this->rb->getTransform().getOrientation();
		this->transform.setRotation({ quat.x, quat.y, quat.z, quat.w });
	}
}

