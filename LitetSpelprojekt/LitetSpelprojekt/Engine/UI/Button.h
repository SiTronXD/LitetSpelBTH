#pragma once
#include "../Application/Input.h"
#include "../Graphics/UIRenderer.h"
#include <iostream>
#include "SimpleMath.h"

class Button
{
private:
	DirectX::SimpleMath::Vector2 pos;
	int width;
	int height;
	UIRenderer& uiRenderer;

public:
	Button(DirectX::SimpleMath::Vector2 p, int w, int h, UIRenderer& r);
	~Button();

	inline DirectX::SimpleMath::Vector2 getPos() const{ return this->pos; }
	inline int getWidth() const{ return this->width; }
	inline int getheight() const { return this->height; }
	inline void setPos(DirectX::SimpleMath::Vector2 newPos) { this->pos = newPos; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }

	bool isClicked();
	void render(std::string textureName);
};