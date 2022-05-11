#include "Settings.h"
#include <fstream>
#include <iostream>
#include "Dev/Log.h"

Settings::Settings()
{
	loadSettings();
}

Settings::~Settings()
{
	saveSettings();
}

//Load in Settings from File
bool Settings::loadSettings()
{
	std::string gameSettings;
	std::ifstream reader;
	
	// Read from settings file
	reader.open("Settings.txt");
	if (!reader.is_open())
	{
		Log::error("Unable to load settings file");
		this->settingsList.resolutionX = 600;
		this->settingsList.resolutionY = 480;
		this->settingsList.sensitivity = 3.0f;
		this->settingsList.brightness = 3.0f;
		this->settingsList.volume = 3.0f;
		this->settingsList.fullscreen = true;
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
		else if (gameSettings == "Sensitivity")
		{
			getline(reader, gameSettings);
			settingsList.sensitivity = stof(gameSettings);
		}
		else if (gameSettings == "Brightness")
		{
			getline(reader, gameSettings);
			settingsList.brightness = stof(gameSettings);
		}
		else if (gameSettings == "Volume")
		{
			getline(reader, gameSettings);
			settingsList.volume = stof(gameSettings);
		}
		else if (gameSettings == "Fullscreen")
		{
			getline(reader, gameSettings);
			settingsList.fullscreen = gameSettings == "true";
		}
	}
	reader.close();

	Log::write("ResX: " + std::to_string(settingsList.resolutionX) +
		"ResY: " + std::to_string(settingsList.resolutionY) +
		"Sens: " + std::to_string(settingsList.sensitivity) +
		"Bright: " + std::to_string(settingsList.brightness) +
		"Volume: " + std::to_string(settingsList.volume) + 
		"Fullscreen: " + (settingsList.fullscreen ? "true" : "false"));
	return true;
}

// Save all settings to file
bool Settings::saveSettings()
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
		<< "\nBrightness\n" << settingsList.brightness
		<< "\nVolume\n" << settingsList.volume
		<< "\nFullscreen\n" << (settingsList.fullscreen ? "true" : "false");

	Log::write("ResX: " + std::to_string(settingsList.resolutionX) +
		"ResY: " + std::to_string(settingsList.resolutionY) +
		"Sens: " + std::to_string(settingsList.sensitivity) +
		"Bright: " + std::to_string(settingsList.brightness) +
		"Volume: " + std::to_string(settingsList.volume) +
		"Fullscreen: " + (settingsList.fullscreen ? "true" : "false"));
	
	writer.close();
	return true;
}

