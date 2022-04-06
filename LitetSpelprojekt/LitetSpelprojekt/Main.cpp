#include "Engine/Engine.h"

int main()
{
	// Set flags for tracking CPU memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;
	engine.run();

	
	return 0;
}