#include "Cycler.h"

using namespace DirectX::SimpleMath;

Cycler::Cycler(Vector2 p, int w, int h, Vector3 bClr, Vector3 hClr, bool hov,
	UIRenderer& r):
	Button(p, w, h, bClr, hClr, hov, r)
{
}

Cycler::~Cycler()
{
}

void Cycler::setElements(std::vector<std::string> elements)
{
	this->elements = elements;
}

void Cycler::setCurrent(std::string current)
{
	this->current = current;
}