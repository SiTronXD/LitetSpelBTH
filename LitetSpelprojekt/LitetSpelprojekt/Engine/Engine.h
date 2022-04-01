#pragma once

#include <Windows.h>
#include <chrono>

class Engine
{
private:
	UINT width;
	UINT height;
public:
	Engine(UINT width, UINT height);
	virtual ~Engine();

	void update(float dt);
	void run();
};

