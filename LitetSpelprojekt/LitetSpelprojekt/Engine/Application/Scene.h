#pragma once

#include "../ECS.h"

class Scene
{
private:
	ECS ecs;

public:
	Scene();
	virtual ~Scene();
};

