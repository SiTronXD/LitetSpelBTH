#include "Button.h"
#include "../Dev/Log.h"
#include <DirectXMath.h>
#include "../ResTranslator.h"
using namespace DirectX::SimpleMath;

Button::Button(Vector2 p, int w, int h, UIRenderer& r):
	uiRenderer(r)
{
	this->pos = p;
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
	int maxPosX = this->pos.x + (this->width / 2);
	int minPosX = this->pos.x - (this->width / 2);
	int maxPosY = this->pos.y + (this->height / 2);
	int minPosY = this->pos.y - (this->height / 2);

	// Transform resolution to internal positions
	DirectX::XMFLOAT2 internal = ResTranslator::toInternalPos(DirectX::XMFLOAT2(Input::getCursorX(), Input::getCursorY()));
	
	// Inside X Range
	if (internal.x >= minPosX && (internal.x <= maxPosX))
	{
		// Inside Y range
		if (internal.y >= minPosY && internal.y <= maxPosY)
		{
			// Left click inside the button
			if (Input::isMouseButtonJustPressed(Mouse::LEFT_BUTTON))
			{
				buttonClicked = true;
			}	
		}
	}
	return buttonClicked;
}

void Button::render(std::string textureName)
{
	uiRenderer.renderTexture(textureName, this->pos.x, this->pos.y, this->width, this->height);
}