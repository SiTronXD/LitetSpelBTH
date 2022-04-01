#include "Settings.h"
#include <fstream>
#include <iostream>
#include "Engine/Dev/Log.h"

Settings::Settings()
{
	this->resolutionX = 0;
	this->resolutionY = 0;
	this->sensitivity = 0;
	this->brightness = 0;
	this->volume = 0;
}

Settings::~Settings()
{

}
bool Settings::LoadSettings()
{
	std::string gameSettings;
	std::ifstream reader;
	
	reader.open("Settings.txt");
	if (!reader.is_open())
	{
		Log::error("Unable to load settings file.\n");
		return false;
	}
	while (getline(reader, gameSettings))
	{
		if (gameSettings == "ResolutionX")
		{
			getline(reader, gameSettings);
			this->resolutionX = stoi(gameSettings);
		}
		else if (gameSettings == "ResolutionY")
		{
			getline(reader, gameSettings);
			this->resolutionY = stoi(gameSettings);
		}
		else if (gameSettings == "Sensitvity")
		{
			getline(reader, gameSettings);
			this->sensitivity = stoi(gameSettings);
		}
		else if (gameSettings == "Brightness")
		{
			getline(reader, gameSettings);
			this->brightness = stoi(gameSettings);
		}
		else if (gameSettings == "Volume")
		{
			getline(reader, gameSettings);
			this->volume = stoi(gameSettings);
		}
	}

	reader.close();
	return true;
}