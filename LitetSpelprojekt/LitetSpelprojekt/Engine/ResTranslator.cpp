#include "ResTranslator.h"

unsigned int ResTranslator::windowWidth = 0;
unsigned int ResTranslator::windowHeight = 0;

void ResTranslator::init(unsigned int _windowWidth, unsigned int _windowHeight)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;
}

UIRectangle ResTranslator::transformRect(
	const UIRectangle& internalRect)
{
	UIRectangle newRect = internalRect;

	float screenMidX = windowWidth / 2.0f;
	float screenMidY = windowHeight / 2.0f;

	float screenSizeScaleY = (float) windowHeight / INTERNAL_HEIGHT;


	// Scale
	newRect.width *= screenSizeScaleY;
	newRect.height *= screenSizeScaleY;

	// Position
	newRect.x = screenMidX + (internalRect.x - (internalRect.width / 2.0f)) * screenSizeScaleY;
	newRect.y = screenMidY + (-internalRect.y - (internalRect.height / 2.0f)) * screenSizeScaleY;

	return newRect;
}
