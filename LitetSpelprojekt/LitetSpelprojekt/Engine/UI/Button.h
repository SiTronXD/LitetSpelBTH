#pragma once
#include "../Application/Input.h"
#include "../Graphics/UIRenderer.h"
#include <iostream>

class Button
{
private:
	int posX;
	int posY;
	int width;
	int height;
	UIRenderer& uiRenderer;

public:
	Button(int pX, int pY, int w, int h, UIRenderer& r);
	~Button();

	inline int getPosX() const{ return this->posX; }
	inline int getPosY() const{ return this->posY; }
	inline int getWidth() const{ return this->width; }
	inline int getheight() const { return this->height; }
	inline void setPosX(int newPosX) { this->posX = newPosX; }
	inline void setPosY(int newPosY) { this->posY = newPosY; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }

	bool isClicked();
	void render(std::string textureName);
};