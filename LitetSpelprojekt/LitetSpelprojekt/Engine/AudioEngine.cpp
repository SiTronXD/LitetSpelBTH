#include "AudioEngine.h"
#include "Dev/Log.h"

AudioEngine::AudioEngine(Resources& resource):
	resource(resource)
{
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::playSound(std::string sound)
{
	sf::SoundBuffer& soundEffect = this->resource.getSoundEffect(sound.c_str());
	this->engine.setBuffer(soundEffect);
	this->engine.play();
}
