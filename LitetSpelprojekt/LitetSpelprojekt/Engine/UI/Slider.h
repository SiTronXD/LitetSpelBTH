#pragma once
#include "../Graphics/UIRenderer.h"
#include "SimpleMath.h"

class Slider
{
private:

	DirectX::SimpleMath::Vector2 pos;
	int width;
	int height;
	int minValue;
	int currentValue;
	int maxValue;
	UIRenderer& uiRenderer;

public:
	Slider(DirectX::SimpleMath::Vector2 p, int w, int h, int minVal, int curVal, int maxVal, UIRenderer& r);
	~Slider();
 
	// Get
	inline DirectX::SimpleMath::Vector2 getPos() const { return this->pos; }
	inline int getWidth() const { return this->width; }
	inline int getheight() const { return this->height; }
	inline int getMinVal() const { return this->maxValue; }
	inline int getCurVal() const { return this->maxValue; }
	inline int getMaxVal() const { return this->maxValue; }
	
	// Set
	inline void setPos(DirectX::SimpleMath::Vector2 newPos) { this->pos = newPos; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }
	inline void setMinVal(int newMinVal) { this->maxValue = newMinVal; }
	inline void setCurVal(int newCurVal) { this->maxValue = newCurVal; }
	inline void setMaxVal(int newMaxVal) { this->maxValue = newMaxVal; }


	bool isClicked();
	void render(std::string textureName);

};