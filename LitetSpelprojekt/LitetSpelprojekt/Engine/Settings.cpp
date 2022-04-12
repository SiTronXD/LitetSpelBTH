#include "Settings.h"
#include <fstream>
#include <iostream>
#include "Dev/Log.h"

Settings::Settings()
{
	this->settingsList.resolutionX = 0;
	this->settingsList.resolutionY = 0;
	this->settingsList.sensitivity = 0;
	this->settingsList.brightness = 0;
	this->settingsList.volume = 0;
}

Settings::~Settings()
{

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
	
	// Read from settings file
	reader.open("Settings.txt");
	if (!reader.is_open())
	{
		Log::write("Unable to load settings file");
		this->settingsList.resolutionX = 1600;
		this->settingsList.resolutionY = 900;
		this->settingsList.sensitivity = 5;
		this->settingsList.brightness = 5;
		this->settingsList.volume = 5;
		return false;
	}

	// Read each value into settings variables
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
	// Open Settings File
	std::ofstream writer;
	writer.open("Settings.txt");
	if (!writer.is_open())
	{
		Log::error("Unable to write to settings file.\n");
		return false;
	}
	
	// Write settings to file
	writer << "ResolutionX\n" << settingsList.resolutionX 
		<< "\nResolutionY\n" << settingsList.resolutionY 
		<< "\nSensitivity\n" << settingsList.sensitivity
		<< "\nBrightnesss\n" << settingsList.brightness
		<< "\nVolume\n" << settingsList.volume;
	
	writer.close();
	return true;
}

