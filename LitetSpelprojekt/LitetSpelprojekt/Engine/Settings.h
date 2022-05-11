#pragma once

struct SettingsStruct
{
	unsigned int resolutionX;
	unsigned int resolutionY;
	float sensitivity;
	float brightness;
	float volume;
	bool fullscreen;
};

class Settings
{
private:
	SettingsStruct settingsList;

public:
	Settings();
	~Settings();
	bool loadSettings();
	bool saveSettings();
	SettingsStruct& getSettings() { return this->settingsList; }

};




