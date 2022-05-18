#pragma once
#include "../Graphics/UIRenderer.h"
#include "SimpleMath.h"
#include "../AudioEngine.h"

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
	DirectX::SimpleMath::Vector3 buttonClr;
	DirectX::SimpleMath::Vector3 changedClr;
	bool changed;
	UIRenderer& uiRenderer;
	AudioEngine& audioEngine;

public:
	Slider(DirectX::SimpleMath::Vector2 p, int w, int h, float minVal, float curVal, float maxVal,
		float percentFilled, DirectX::SimpleMath::Vector3 bClr,
		DirectX::SimpleMath::Vector3 cClr, bool cha, UIRenderer& r, AudioEngine& audio);
	~Slider();
 
	// Get
	inline DirectX::SimpleMath::Vector2 getPos() const { return this->pos; }
	inline int getWidth() const { return this->width; }
	inline int getheight() const { return this->height; }
	inline float getMinVal() const { return this->minValue; }
	inline float getCurVal() const { return this->currentValue; }
	inline float getMaxVal() const { return this->maxValue; }
	inline float getPerFill() const { return this->percentFilled; }
	
	// Set
	inline void setPos(DirectX::SimpleMath::Vector2 newPos) { this->pos = newPos; }
	inline void setWidth(int newWidth) { this->width = newWidth; }
	inline void setHeight(int newHeight) { this->height = newHeight; }
	inline void setMinVal(float newMinVal) { this->maxValue = newMinVal; }
	inline void setCurVal(float newCurVal) { this->maxValue = newCurVal; }
	inline void setMaxVal(float newMaxVal) { this->maxValue = newMaxVal; }
	inline void setPerFill(float newPerFill) { this->percentFilled = newPerFill; }

	bool isClicked();
	void render(std::string textureName);

};