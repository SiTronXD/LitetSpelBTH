#pragma once

class Settings
{
private:

	unsigned int resolutionX;
	unsigned int resolutionY;
	unsigned int sensitivity;
	unsigned int brightness;
	unsigned int volume;

public:

	Settings();
	~Settings();
	bool LoadSettings();

};

