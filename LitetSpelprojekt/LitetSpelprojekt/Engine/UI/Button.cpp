#include "Button.h"
#include "../Dev/Log.h"


Button::Button(int pX, int pY, int w, int h, UIRenderer& r) : uiRenderer(r)
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
			if (Input::isMouseButtonDown(Mouse::LEFT_BUTTON))
			{
				buttonClicked = true;
			}	
		}
	}
	return buttonClicked;
}

void Button::render(std::string textureName)
{
	uiRenderer.renderTexture(textureName, this->posX, this->posY, this->width, this->height);
}