#pragma once
#include "../Graphics/UIRenderer.h"
#include "SimpleMath.h"

class Slider
{
private:
	DirectX::SimpleMath::Vector2 pos;
	int width;
	int height;
	float minValue;
	float currentValue;
	float maxValue;
	float percentFilled;
	UIRenderer& uiRenderer;

public:
	Slider(DirectX::SimpleMath::Vector2 p, int w, int h, float minVal, float curVal, float maxVal, float percentFilled, UIRenderer& r);
	~Slider();
 
	// Get
	inline DirectX::SimpleMath::Vector2 getPos() const { return this->pos; }
	inline int getWidth() const { return this->width; }
	inline int getheight() const { return this->height; }
	inline float getMinVal() const { return this->maxValue; }
	inline float getCurVal() const { return this->maxValue; }
	inline float getMaxVal() const { return this->maxValue; }
	inline float getPerFil() const { return this->percentFilled; }
	
	// Set
	inline void setPos(DirectX::SimpleMath::Vector2 newPos) { this->pos = newPos; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }
	inline void setMinVal(float newMinVal) { this->maxValue = newMinVal; }
	inline void setCurVal(float newCurVal) { this->maxValue = newCurVal; }
	inline void setMaxVal(float newMaxVal) { this->maxValue = newMaxVal; }
	inline void setPerFil(float newPerFil) { this->percentFilled = newPerFil; }

	bool isClicked();
	void render(std::string textureName);

};