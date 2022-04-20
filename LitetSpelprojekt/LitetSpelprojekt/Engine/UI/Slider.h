#pragma once
#include "../Graphics/UIRenderer.h"

class Slider
{
private:
	int posX;
	int posY;
	int width;
	int height;
	int maxValue;
	UIRenderer& uiRenderer;

public:
	Slider(int pX, int pY, int w, int h, int maxVal, UIRenderer& r);
	~Slider();
 
	inline int getPosX() const { return this->posX; }
	inline int getPosY() const { return this->posY; }
	inline int getWidth() const { return this->width; }
	inline int getheight() const { return this->height; }
	inline int getMaxVal() const { return this->maxValue; }
	inline void setPosX(int newPosX) { this->posX = newPosX; }
	inline void setPosY(int newPosY) { this->posY = newPosY; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }
	inline void setMaxVal(int newMaxVal) { this->maxValue = newMaxVal; }

	bool isClicked();
	void render(std::string textureName);

};