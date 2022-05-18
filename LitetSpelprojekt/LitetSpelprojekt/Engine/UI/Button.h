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
	DirectX::SimpleMath::Vector3 buttonClr;
	DirectX::SimpleMath::Vector3 hoverClr;
	bool hovered;
	UIRenderer& uiRenderer;

public:
	Button(DirectX::SimpleMath::Vector2 p, int w, int h, DirectX::SimpleMath::Vector3 bClr,
		DirectX::SimpleMath::Vector3 hClr, bool hov, UIRenderer& r);
	~Button();

	inline DirectX::SimpleMath::Vector2 getPos() const { return this->pos; }
	inline int getWidth() const{ return this->width; }
	inline int getheight() const { return this->height; }
	inline void setPos(DirectX::SimpleMath::Vector2 newPos) { this->pos = newPos; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }

	bool isClicked();
	void render(std::string textureName);

protected:
	inline UIRenderer& getUIRenderer() const { return this->uiRenderer; }
};