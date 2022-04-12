#pragma once

class Button
{
private:
	int width;
	int height;

public:
	Button();
	~Button();

	inline int getWidth() { return this->width; }
	inline int getheight() { return this->height; }
	bool isClicked();
};