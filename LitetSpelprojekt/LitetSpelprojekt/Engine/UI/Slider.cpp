#include "Slider.h"
#include "../ResTranslator.h"
#include "../Application/Input.h"
#include "../Dev/Log.h"

using namespace DirectX::SimpleMath;

Slider::Slider(Vector2 p, int w, int h, float minVal, float curVal, float maxVal, UIRenderer& r) :
	uiRenderer(r)
{
	this->pos = p;
	this->width = w;
	this->height = h;
	this->minValue = minVal;
	this->currentValue = curVal;
	this->maxValue = maxVal;
}

Slider::~Slider()
{
}

bool Slider::isClicked()
{
	bool sliderClicked = false;

	// Slider boundries
	int maxPosX = this->pos.x + (this->width / 2);
	int minPosX = this->pos.x - (this->width / 2);
	int maxPosY = this->pos.y + (this->height / 2);
	int minPosY = this->pos.y - (this->height / 2);

	// Transform resolution to internal positions
	DirectX::XMFLOAT2 internal = ResTranslator::toInternalPos(DirectX::XMFLOAT2(Input::getCursorX(), Input::getCursorY()));
	float test = (float)internal.x;
	// Inside X Range
	if (internal.x >= minPosX && (internal.x <= maxPosX))
	{
		// Inside Y range
		if (internal.y >= minPosY && internal.y <= maxPosY)
		{
			// Left click inside the button
			if (Input::isMouseButtonJustPressed(Mouse::LEFT_BUTTON))
			{
				//this->currentValue = internal.x;
				Log::write("Internal X: " + std::to_string(internal.x));
				sliderClicked = true;
			}
		}
	}
	return sliderClicked;
}

void Slider::render(std::string textureName)
{
	float percent = this->currentValue / this->maxValue;
	uiRenderer.renderTexture(textureName, this->pos.x - ((width - this->width * percent) / 2), this->pos.y, this->width * percent, this->height);
	uiRenderer.renderTexture("healthBar.png", this->pos.x, this->pos.y, this->width, this->height);
}
