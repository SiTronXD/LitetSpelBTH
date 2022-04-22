#pragma once
#include "Texture.h"
#include <string.h>
class CubeMap
{
private:
	Texture texture;
public:
	CubeMap(Renderer& renderer);
	~CubeMap();

	inline Texture& getTexture() { return this->texture; }

	void createCubeMap(std::string path);
};

