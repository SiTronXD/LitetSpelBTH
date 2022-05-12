#pragma once

#include "SFML/Audio.hpp"
#include "Resources.h"

class AudioEngine
{
private:
	sf::Sound engine;
	Resources& resource;
public:
	AudioEngine(Resources& resource);
	virtual ~AudioEngine();

	void playSound(std::string sound);
};

