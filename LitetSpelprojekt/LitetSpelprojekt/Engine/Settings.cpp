#include "Settings.h"
#include <fstream>
#include <iostream>
#include "Dev/Log.h"

Settings::Settings()
{
	LoadSettings();
}

Settings::~Settings()
{
	SaveSettings();
}

SettingsStruct& Settings::getSettings()
{
	return this->settingsList;
}

//Load in Settings from File
bool Settings::LoadSettings()
{
	std::string gameSettings;
	std::ifstream reader;
	
	reader.open("Engine/Settings.txt");
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
			settingsList.resolutionX = stoi(gameSettings);
		}
		else if (gameSettings == "ResolutionY")
		{
			getline(reader, gameSettings);
			settingsList.resolutionY = stoi(gameSettings);
		}
		else if (gameSettings == "Sensitvity")
		{
			getline(reader, gameSettings);
			settingsList.sensitivity = stoi(gameSettings);
		}
		else if (gameSettings == "Brightness")
		{
			getline(reader, gameSettings);
			settingsList.brightness = stoi(gameSettings);
		}
		else if (gameSettings == "Volume")
		{
			getline(reader, gameSettings);
			settingsList.volume = stoi(gameSettings);
		}
	}
	reader.close();
	return true;
}

// Save all settings to file
bool Settings::SaveSettings()
{
	std::string gameSettings;
	std::ofstream writer;

	writer.open("Settings.txt");
	if (!writer.is_open())
	{
		Log::error("Unable to write to file.\n");
		return false;
	}
	
	writer << "ResolutionX\n" << settingsList.resolutionX 
		<< "\nResolutionY\n" << settingsList.resolutionY 
		<< "\nSensitivity\n" << settingsList.sensitivity
		<< "\nBrightnesss\n" << settingsList.brightness
		<< "\nVolume\n" << settingsList.volume;
	
	writer.close();
	return true;
}

