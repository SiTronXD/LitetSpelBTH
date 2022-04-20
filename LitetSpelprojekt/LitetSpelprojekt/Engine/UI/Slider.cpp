#include "Slider.h"

Slider::Slider(int maxVal, int pX, int pY, int w, int h, UIRenderer& r):
	uiRenderer(r)
{
	this->maxValue = maxVal;
	this->width = pX;
	this->height = pY;
	this->posX = w;
	this->posY = h;
}

Slider::~Slider()
{
}

bool Slider::isClicked()
{


	return true;
}

void Slider::Render()
{
}
