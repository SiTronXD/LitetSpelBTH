#pragma once

#include "../Ecs.h"

class Scene
{
private:
	Ecs ecs;

public:
	Scene();
	virtual ~Scene();
};

