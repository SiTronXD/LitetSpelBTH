#include "Rigidbody.h"
#include "../GameObject.h"
#include "../Time.h"

using namespace DirectX::SimpleMath;

Rigidbody::Rigidbody(GameObject& object):
	Script(object),
	transform(*this->getTransform()),
	mass(-1.0f),
	isKinematic(false)
{
	this->acceleration = Vector3(0.0f, -9.82f, 0.0f);
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::init()
{
}

void Rigidbody::update()
{
	// Adjust position by velocity
	transform.move(this->velocity * Time::getDT());

	// Adjust velocity by acceleration. Do this last because
	// velocity could be manually set.
	this->velocity += this->acceleration * Time::getDT();
}

void Rigidbody::onCollisionStay(GameObject& other)
{
	Rigidbody* otherRb = other.getComponent<Rigidbody>();

	if (otherRb)
	{
		float e = 0.0f;

		this->velocity = (this->velocity * (this->mass - otherRb->mass * e) +
			otherRb->velocity * otherRb->mass * (1.0f + e)) / 
			(this->mass + otherRb->mass);
	}
}

void Rigidbody::setVelocity(const DirectX::SimpleMath::Vector3& newVelocity)
{
	this->velocity = newVelocity;
}

void Rigidbody::addForce(const DirectX::SimpleMath::Vector3& newForce)
{
	this->velocity += newForce;
}
