#pragma once
#include <iostream>
struct HighscoreList
{
	float times[10];
};

class Highscore
{
private:
	HighscoreList highscoreList;

public:
	Highscore();
	~Highscore();

	inline HighscoreList& getHighscoreList() { return this->highscoreList; }
	bool loadHighscore();
	bool saveHighscore();
	void displayHighscore() const;
	bool newHighscore(float time);
};