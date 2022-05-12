#include "Highscore.h"
#include "Dev/Log.h"
#include <fstream>
#include <Bits.h>
#include <algorithm>

Highscore::Highscore()
{
	for (int i = 0; i < 10; i++)
	{
		this->highscoreList.times[i] = 99.9999f;
	}
}

Highscore::~Highscore()
{
}

bool Highscore::loadHighscore()
{
	std::string readHighscore;
	std::ifstream reader;
	reader.open("Highscore.txt");
	if (!reader.is_open())
	{
		Log::error("Unable to open highscore.txt");
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		getline(reader, readHighscore);
		this->highscoreList.times[i] = stof(readHighscore);
	}
	reader.close();
	return true;
}

bool Highscore::saveHighscore()
{
	std::ofstream writer;
	writer.open("Highscore.txt");
	if (!writer.is_open())
	{
		Log::error("Unable to write to highscore file.\n");
		return false;
	}

	for (int i = 0; i < 10; i++)
	{
		writer << highscoreList.times[i] << std::endl;
	}
	writer.close();
	return true;
}

void Highscore::displayHighscore() const
{
	for (int i = 0; i < 10; i++)
	{
		Log::write(std::to_string((i+1)) + ": " +  std::to_string(this->highscoreList.times[i]));
	}
}

bool Highscore::newHighscore(float time)
{
	if (time == 0.0f)
	{
		return false;
	}

	this->highscoreList.times[10] = time;
	std::sort(this->highscoreList.times, this->highscoreList.times + 11);
	if (time == this->highscoreList.times[10])
	{
		return false;
	}
	
	/* DEBUGGING
	Log::write("Time: " + std::to_string(time));
	Log::write("First Time: " + std::to_string(this->highscoreList.times[0]));
	Log::write("Last Time: " + std::to_string(this->highscoreList.times[9]));
	Log::write("11th: " + std::to_string(this->highscoreList.times[10]));
	*/


	if (!saveHighscore())
	{
		Log::error("Save Highscore returned false");
	}

	return true;
}
