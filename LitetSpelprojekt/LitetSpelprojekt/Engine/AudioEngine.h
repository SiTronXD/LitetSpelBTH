#pragma once

#include "SFML/Audio.hpp"
#include "Resources.h"

class AudioEngine
{
private:
	static const unsigned int MAX_NUM_SOUNDS = 5;

	unsigned int currentEngine;

	bool active;

	sf::Sound engine[MAX_NUM_SOUNDS];
	sf::Music music;

	std::string curMusicPath;

	Resources& resource;
public:
	AudioEngine(Resources& resource);
	virtual ~AudioEngine();

	void setVolume(float percent);
	void playSound(std::string sound);
	void setMusic(std::string path);
};

