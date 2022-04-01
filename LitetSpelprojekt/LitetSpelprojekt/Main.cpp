#include <iostream>

#include "Engine/Application/Window.h"

int main()
{
	Window window(1600, 900, "Litet Spelprojekt");
	window.init();

	while (window.isRunning())
	{

	}

	return 0;
}