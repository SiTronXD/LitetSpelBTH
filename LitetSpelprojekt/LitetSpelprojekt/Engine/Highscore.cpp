#include "Highscore.h"
#include "Dev/Log.h"
#include <fstream>

Highscore::Highscore()
{
	
	
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
		
		Log::write((i+1) + ":" + std::to_string(this->highscoreList.times[i]));
	}
}

bool Highscore::newHighscore(float time)
{
	for (int i = 9; i > 0; i--)
	{
		if (time < this->highscoreList.times[i])
		{
			return false;
		}

	}
	
	return true;
}