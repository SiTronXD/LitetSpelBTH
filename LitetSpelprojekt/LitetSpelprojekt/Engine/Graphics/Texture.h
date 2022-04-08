#pragma once

#include <string>
#include <d3d11.h>

class Renderer;

class Texture
{
private:
	ID3D11SamplerState* samplerState;
	ID3D11Texture2D* texture;

	Renderer& renderer;

	unsigned int width;
	unsigned int height;

	bool createSampler();

public:
	Texture(Renderer& renderer);
	virtual ~Texture();

	bool load(const std::string& fileName);
};