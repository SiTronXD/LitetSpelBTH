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

	float screenMidX = windowWidth * 0.5f;
	float screenMidY = windowHeight * 0.5f;

	float screenSizeScaleY = (float) windowHeight / INTERNAL_HEIGHT;


	// Scale
	newRect.width *= screenSizeScaleY;
	newRect.height *= screenSizeScaleY;

	// Position
	newRect.x = screenMidX + (internalRect.x - (internalRect.width * 0.5f)) * screenSizeScaleY;
	newRect.y = screenMidY + (-internalRect.y - (internalRect.height * 0.5f)) * screenSizeScaleY;

	return newRect;
}
