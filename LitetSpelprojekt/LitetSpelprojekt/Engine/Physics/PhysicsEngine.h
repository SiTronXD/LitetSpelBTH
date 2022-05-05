#pragma once

#include "../ECS.h"
#include "../Application/Scene.h"
#include "PhysicsEventListener.h"
#include "PhysicsRaycastCallback.h"

class PhysicsEngine
{
private:
	static const float TIMESTEP;
	static float accumulator;

	rp3d::PhysicsCommon physCom;
	rp3d::PhysicsWorld* world;
	PhysicsEventListener listener;

	Scene* scene;
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	void init(Scene* scene);

	rp3d::PhysicsCommon& getCommon();
	rp3d::PhysicsWorld* getWorld();

	void update();
	RaycastInfo raycast(rp3d::Ray ray);

	static const float& getInterpolationFactor()
		{ return accumulator / TIMESTEP; }
};