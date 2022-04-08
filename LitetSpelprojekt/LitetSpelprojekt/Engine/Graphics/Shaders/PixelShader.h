#pragma once

#include "Shader.h"

class PixelShader : public Shader
{
private:
	ID3D11PixelShader* pixelShader;

public:
	PixelShader(Renderer& renderer);
	virtual ~PixelShader();

	bool loadPS(const std::string fileName);

	inline ID3D11PixelShader*& getPS() { return this->pixelShader; }
};