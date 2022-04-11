#include "Engine/Engine.h"
#include "Settings.h"

int main()
{
	// Set flags for tracking CPU memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine(1280, 720);
	engine.run();

	Settings settings;
	settings.LoadSettings();
	
	return 0;
}