#pragma once

#include "SFML/Audio.hpp"
#include "Resources.h"

class AudioEngine
{
private:
	bool active;
	sf::Sound engine;
	Resources& resource;
public:
	AudioEngine(Resources& resource);
	virtual ~AudioEngine();

	void setVolume(float percent);
	void playSound(std::string sound);
};

