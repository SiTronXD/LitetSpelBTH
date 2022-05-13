#include "AudioEngine.h"
#include "Dev/Log.h"
#include <iostream>

AudioEngine::AudioEngine(Resources& resource):
	resource(resource), active(true)
{
	if (!sf::SoundRecorder::getAvailableDevices().size())
		this->active = false;
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::setVolume(float percent)
{
	this->engine.setVolume(100 * percent);
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
