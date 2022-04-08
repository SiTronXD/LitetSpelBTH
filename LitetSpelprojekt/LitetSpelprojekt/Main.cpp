#include "Engine/Engine.h"
#include "Settings.h"

// Remove below
#include "Engine/Ecs.h"

int main()
{
	// Set flags for tracking CPU memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*Engine engine(1280, 720);
	engine.run();

	Settings settings;
	settings.LoadSettings();*/

	ECS ecs;
	GameObject& g = ecs.addGameObject();
	ecs.addComponent<Light>(g);
	ecs.addComponent<Camera>(g);
	ecs.removeComponent<Light>(g);
	ecs.addComponent<Light>(g);
	Collider* t = ecs.getComponent<Collider>(g);
	
	return 0;
}