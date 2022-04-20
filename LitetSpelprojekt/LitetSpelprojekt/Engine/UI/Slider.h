#pragma once
#include "../Graphics/UIRenderer.h"

class Slider
{
private:
	int maxValue;
	int width;
	int height;
	int posX;
	int posY;
	UIRenderer& uiRenderer;
public:
	Slider(int maxVal, int pX, int pY, int w, int h, UIRenderer& r);
	~Slider();
	bool isClicked();
	void Render();

};