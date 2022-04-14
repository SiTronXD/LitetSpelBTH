#pragma once
#include "../Application/Input.h"

class Button
{
private:
	int posX;
	int posY;
	int width;
	int height;

public:
	Button();
	Button(int posX, int posY, int w, int h);
	~Button();

	inline int getPosX() { return this->posX; }
	inline int getPosY() { return this->posY; }
	inline int getWidth() { return this->width; }
	inline int getheight() { return this->height; }

	bool isClicked();
};