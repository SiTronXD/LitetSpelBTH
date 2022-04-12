#pragma once

struct SettingsStruct
{
	unsigned int resolutionX;
	unsigned int resolutionY;
	unsigned int sensitivity;
	unsigned int brightness;
	unsigned int volume;
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




