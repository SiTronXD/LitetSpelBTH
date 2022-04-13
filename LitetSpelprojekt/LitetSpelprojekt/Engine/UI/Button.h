#pragma once
#include "../Application/Input.h"

class Button
{
private:
	int width;
	int height;
	int pos[2];
public:
	Button();
	Button(int w, int h, int[] pos);
	~Button();

	inline int getWidth() { return this->width; }
	inline int getheight() { return this->height; }
	inline int getPosition() { return this->pos; }
	bool isClicked();
};