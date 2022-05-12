#include "Button.h"
#include "../Dev/Log.h"
#include <DirectXMath.h>
#include "../ResTranslator.h"
using namespace DirectX::SimpleMath;

Button::Button(Vector2 p, int w, int h, Vector3 bClr, Vector3 hClr, bool hov, UIRenderer& r):
	uiRenderer(r)
{
	this->pos = p;
	this->width = w;
	this->height = h;
	this->buttonClr = bClr;
	this->hoverClr = hClr;
	this->hovered = hov;
}

Button::~Button()
{
}

// Determine if the mouse click is inside of the area of the button
bool Button::isClicked()
{
	// If inside the button
	bool buttonClicked = false;
	this->hovered = false;
	// Button boundries
	int maxPosX = this->pos.x + (this->width / 2.0);
	int minPosX = this->pos.x - (this->width / 2.0);
	int maxPosY = this->pos.y + (this->height / 2.0);
	int minPosY = this->pos.y - (this->height / 2.0);

	// Transform resolution to internal positions
	DirectX::XMFLOAT2 internal = ResTranslator::toInternalPos(DirectX::XMFLOAT2(Input::getCursorX(), Input::getCursorY()));
	
	// Inside X Range
	if (internal.x >= minPosX && (internal.x <= maxPosX))
	{
		// Inside Y range
		if (internal.y >= minPosY && internal.y <= maxPosY)
		{
			hovered = true;
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
	if (!hovered)
		uiRenderer.renderTexture(textureName, this->pos.x, this->pos.y, this->width, this->height, this->buttonClr);
	else
		uiRenderer.renderTexture(textureName, this->pos.x, this->pos.y, this->width, this->height, this->hoverClr);
	
}