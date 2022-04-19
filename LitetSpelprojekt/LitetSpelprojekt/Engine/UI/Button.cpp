#include "Button.h"
#include "../Dev/Log.h"
#include <DirectXMath.h>
#include "../ResTranslator.h"


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
	// If inside the button
	bool buttonClicked = false;

	// Button boundries
	int maxPosX = this->posX + (this->width / 2);
	int minPosX = this->posX - (this->width / 2);
	int maxPosY = this->posY + (this->height / 2);
	int minPosY = this->posY - (this->height / 2);

	// Transform resolution to internal positions
	DirectX::XMFLOAT2 internal = ResTranslator::toInternalPos(DirectX::XMFLOAT2(Input::getCursorX(), Input::getCursorY()));
	
	/*  DEBUG
	int eX = Input::getCursorX();
	int eY = Input::getCursorY();
	Log::write("minPosX: " + std::to_string(minPosX));
	Log::write("maxPosX: " + std::to_string(maxPosX));
	Log::write("minPosY: " + std::to_string(minPosY));
	Log::write("maxPosY: " + std::to_string(maxPosY));
	Log::write("internalX: " + std::to_string(internal.x));
	Log::write("internalY: " + std::to_string(internal.y));
	*/

	// Inside X Range
	if (internal.x >= minPosX && (internal.x <= maxPosX))
	{
		// Inside Y range
		if (internal.y >= minPosY && internal.y <= maxPosY)
		{
			// Left click inside the button
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