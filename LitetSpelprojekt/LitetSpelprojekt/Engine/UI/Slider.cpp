#include "Slider.h"
#include "../ResTranslator.h"
#include "../Application/Input.h"
#include "../Dev/Log.h"

using namespace DirectX::SimpleMath;

Slider::Slider(Vector2 p, int w, int h, float minVal, float curVal, float maxVal, float perFil, Vector3 bClr, Vector3 cClr, bool cha, UIRenderer& r, AudioEngine& audio) :
	uiRenderer(r), audioEngine(audio)
{
	this->pos = p;
	this->width = w;
	this->height = h;
	this->minValue = minVal;
	this->currentValue = curVal;
	this->maxValue = maxVal;
	this->percentFilled = perFil;
	this->buttonClr = bClr;
	this->changedClr = cClr;
	this->changed = cha;
}

Slider::~Slider()
{
}

bool Slider::isClicked()
{
	bool sliderClicked = false;
	this->changed = false;

	// Slider boundries
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
			if (Input::isMouseButtonJustPressed(Mouse::LEFT_BUTTON))
				this->audioEngine.playSound("MenuSlider");

			// Left click inside the button
			if (Input::isMouseButtonDown(Mouse::LEFT_BUTTON))
			{
				this->percentFilled = (float)((this->width / 2.0 + internal.x) / this->width);
				this->currentValue = this->percentFilled * (this->maxValue - this->minValue);
				sliderClicked = true;
				this->changed = true;
			}
		}
	}
	return sliderClicked;
}

void Slider::render(std::string textureName)
{
	// Render Filled out Slider texture
	if (changed)
	{
		uiRenderer.renderTexture(
			textureName,
			(int)(this->pos.x - ((this->width - this->width * this->percentFilled) / 2)),
			(int)this->pos.y,
			(int)(this->width * this->percentFilled),
			this->height,
			this->changedClr
		);
	}
	else
	{
		uiRenderer.renderTexture(
			textureName,
			(int)(this->pos.x - ((this->width - this->width * this->percentFilled) / 2)),
			(int)this->pos.y,
			(int)(this->width * this->percentFilled),
			this->height,
			this->buttonClr
		);
	}

	// Render Slider border
	uiRenderer.renderTexture(
		"sliderBorder.png",
		(int)this->pos.x,
		(int)this->pos.y,
		this->width,
		this->height);
}
