#include "AudioEngine.h"
#include "Dev/Log.h"
#include <iostream>

AudioEngine::AudioEngine(Resources& resource):
	resource(resource), active(true)
{
	if (!sf::SoundRecorder::getAvailableDevices().size())
		this->active = false;
	this->music.setLoop(true);
	this->curMusicPath = "";
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::setVolume(float percent)
{
	this->engine.setVolume(100 * percent);
	this->music.setVolume(100 * percent);
}

void AudioEngine::playSound(std::string sound)
{
	if (this->active)
	{
		sf::SoundBuffer& soundEffect = this->resource.getSoundEffect(sound.c_str());
		this->engine.setBuffer(soundEffect);
		this->engine.play();
	}
}

void AudioEngine::setMusic(std::string path)
{
	if (path == this->curMusicPath)
		return;

	this->curMusicPath = path;
	if (this->music.openFromFile(path))
		this->music.play();
}
