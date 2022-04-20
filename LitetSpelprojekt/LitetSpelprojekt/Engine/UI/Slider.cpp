#include "Slider.h"

Slider::Slider(int pX, int pY, int w, int h, int maxVal, UIRenderer& r):
	uiRenderer(r)
{
	this->posX = w;
	this->posY = h;
	this->width = pX;
	this->height = pY;
	this->maxValue = maxVal;
}

Slider::~Slider()
{
}

bool Slider::isClicked()
{

	return true;
}

void Slider::render(std::string textureName)
{
	uiRenderer.renderTexture(textureName, this->posX, this->posY, this->width, this->height);
}
