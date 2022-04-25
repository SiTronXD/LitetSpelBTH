#include "Rigidbody.h"
#include "../GameObject.h"
#include "../Time.h"

using namespace DirectX::SimpleMath;

Rigidbody::Rigidbody(GameObject& object) :
	Script(object),
	transform(*this->getTransform()),
	mass(1.0f),
	kinematicStatus(false)
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
	if (!this->kinematicStatus)
	{
		// Adjust position by velocity
		transform.move(this->velocity * Time::getDT());

		// Adjust velocity by acceleration. Do this last because
		// velocity could be manually set.
		this->velocity += this->acceleration * Time::getDT();
	}
}

void Rigidbody::onCollisionEnter(GameObject& other)
{
	Rigidbody* otherRb = other.getComponent<Rigidbody>();

	if (otherRb)
	{
		float e = 1.0f;
		this->velocity = -(this->velocity * (this->mass - otherRb->mass * e) +
			otherRb->velocity * otherRb->mass * (1.0f + e)) /
			(this->mass + otherRb->mass);
	}
}

void Rigidbody::onCollisionStay(GameObject& other)
{
	Rigidbody* otherRb = other.getComponent<Rigidbody>();

	if (otherRb)
	{
		float e = 1.0f;
		this->velocity = -(this->velocity * (this->mass - otherRb->mass * e) +
			otherRb->velocity * otherRb->mass * (1.0f + e)) /
			(this->mass + otherRb->mass);
	}
}

void Rigidbody::onCollisionExit(GameObject& other)
{
}

bool Rigidbody::isKinematic() const
{
	return this->kinematicStatus;
}

void Rigidbody::setKinematicStatus(bool kinematicStatus)
{
	this->kinematicStatus = kinematicStatus;
}

void Rigidbody::setMass(float newMass)
{
	this->mass = newMass;
}

DirectX::SimpleMath::Vector3 Rigidbody::getVelocity() const
{
	return this->velocity;
}

void Rigidbody::setVelocity(const DirectX::SimpleMath::Vector3& newVelocity)
{
	this->velocity = newVelocity;
}

void Rigidbody::addForce(const DirectX::SimpleMath::Vector3& newForce)
{
	this->velocity += newForce;
}
