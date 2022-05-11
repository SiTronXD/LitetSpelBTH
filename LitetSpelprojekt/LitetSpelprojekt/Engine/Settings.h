#pragma once

struct SettingsStruct
{
	unsigned int resolutionX;
	unsigned int resolutionY;
	float graphics;
	float sensitivity;
	float volume;
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
	inline SettingsStruct& getSettings() { return this->settingsList; }
};




