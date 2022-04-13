#include "Button.h"

Button::Button()
{
	this->w = 0;
	this->h = 0;
	this->pos[0] = 0;
	this->pos[1] = 0;
}

Button::Button(int w = 0, int h = 0, int[] p)
{
	this->width = w;
	this->height = h;
	this->pos[0] = p[0];
	this->pos[1] = p[1];
}

Button::~Button()
{
}

bool Button::isClicked()
{
	bool buttonClicked = false;

	int maxPosX = pos[0] + (width / 2);
	int minPosX = pos[0] - (width / 2);
	int maxPosY = pos[1] + (height / 2);
	int minPosY = pos[1] + (height / 2);

	if (Input::getCursorX() >= minPosX && (Input::getCursorX() <= maxPosX))
	{
		if (Input::getCursorY() >= minPosY && Input::getCursorY() <= maxPosY)
		{
			buttonClicked = true;
		}
	}

	return buttonClicked;
}