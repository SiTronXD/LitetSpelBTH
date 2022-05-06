#include "PhysicsEngine.h"
#include "../GameObject.h"
#include "../Time.h"

const float PhysicsEngine::TIMESTEP = 1.0f / 60.0f;
float PhysicsEngine::accumulator = 0.0f;

PhysicsEngine::PhysicsEngine():
	scene(nullptr)
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
	std::vector<Rigidbody*> rigidbodies = this->scene->getECS().getActiveComponents<Rigidbody>();
	this->listener.updateRigidbodyVector(rigidbodies);

	PhysicsEngine::accumulator += Time::getDT();
	while (accumulator >= this->TIMESTEP)
	{
		this->world->update(this->TIMESTEP);
		PhysicsEngine::accumulator -= this->TIMESTEP;

		// Update rigidbody states
		for (unsigned int i = 0; i < rigidbodies.size(); ++i)
			rigidbodies[i]->updateStates();
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