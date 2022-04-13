#include "Button.h"

Button::Button()
{
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
}

Button::Button(int pX, int pY, int w, int h)
{
	this->posX = pX;
	this->posY = pY;
	this->width = w;
	this->height = h;
}

Button::~Button()
{
}

// Determine if the mouse click is inside of the area of the button
bool Button::isClicked()
{
	bool buttonClicked = false;

	int maxPosX = this->posX + (this->width / 2);
	int minPosX = this->posX - (this->width / 2);
	int maxPosY = this->posY + (this->height / 2);
	int minPosY = this->posY + (this->height / 2);

	if (Input::getCursorX() >= minPosX && (Input::getCursorX() <= maxPosX))
	{
		if (Input::getCursorY() >= minPosY && Input::getCursorY() <= maxPosY)
		{
			buttonClicked = true;
		}
	}

	return buttonClicked;
}