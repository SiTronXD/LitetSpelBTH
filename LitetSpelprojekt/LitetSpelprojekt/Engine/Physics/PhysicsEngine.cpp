#include "PhysicsEngine.h"
#include "../GameObject.h"
#include "../Time.h"

PhysicsEngine::PhysicsEngine():
	accumulator(0.0f), scene(nullptr)
{
	this->world = this->physCom.createPhysicsWorld();
	this->world->setEventListener(&this->listener);
}

PhysicsEngine::~PhysicsEngine()
{
	this->physCom.destroyPhysicsWorld(this->world);
}

void PhysicsEngine::init(Scene* scene)
{
	this->scene = scene;
}

rp3d::PhysicsCommon& PhysicsEngine::getCommon()
{
	return this->physCom;
}

rp3d::PhysicsWorld* PhysicsEngine::getWorld()
{
	return this->world;
}

void PhysicsEngine::update()
{
	this->listener.updateRigidbodyVector(this->scene->getECS().getActiveComponents<Rigidbody>());

	this->accumulator += Time::getDT();
	while (this->accumulator >= this->TIMESTEP)
	{
		this->world->update(this->TIMESTEP);
		this->accumulator -= this->TIMESTEP;
	}
}

RaycastInfo PhysicsEngine::raycast(rp3d::Ray ray)
{
	RaycastInfo info;
	PhysicsRaycastCallback callback(info, this->scene->getECS().getActiveComponents<Rigidbody>());
	info.distance = (ray.point1 - ray.point2).length();

	this->world->raycast(ray, &callback);

	return info;
}