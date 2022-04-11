#pragma once

#include <string>
#include <d3d11.h>

#include "SRV.h"

class Renderer;

class Texture
{
private:
	ID3D11SamplerState* samplerState;
	ID3D11Texture2D* texture;

	SRV textureSRV;

	Renderer& renderer;

	unsigned int width;
	unsigned int height;

	bool createSampler();

public:
	Texture(Renderer& renderer);
	virtual ~Texture();

	bool load(const std::string& fileName);

	inline SRV& getSRV() { return this->textureSRV; }
	inline ID3D11SamplerState*& getSampler() { return this->samplerState; }
};