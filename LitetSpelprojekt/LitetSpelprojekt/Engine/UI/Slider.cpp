#include "Slider.h"
using namespace DirectX::SimpleMath;

Slider::Slider(Vector2 p, int w, int h, int minVal, int curVal, int maxVal, UIRenderer& r) :
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

	return true;
}

void Slider::render(std::string textureName)
{
	uiRenderer.renderTexture(textureName, this->pos.x, this->pos.y, this->width, this->height);
}
