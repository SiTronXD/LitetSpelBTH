#pragma once

struct UIRectangle
{
	int x;
	int y;
	int width;
	int height;
};

class ResTranslator
{
private:
	friend class Engine;

	static const unsigned int INTERNAL_WIDTH = 1920;
	static const unsigned int INTERNAL_HEIGHT = 1080;

	static unsigned int windowWidth;
	static unsigned int windowHeight;

	static void init(
		unsigned int _windowWidth, unsigned int _windowHeight
	);

public:
	static UIRectangle transformRect(
		const UIRectangle& internalRect
	);
};