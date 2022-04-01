#include "Engine/Engine.h"
#include "Engine/ECS.h"

int main()
{
	Engine engine(1280, 720);
	engine.run();

	ECS ecs;
	GameObject gameObject(ecs);
	ecs.addGameObject(gameObject);


	//gameObject.addComponent<Transform>();

	return 0;
}