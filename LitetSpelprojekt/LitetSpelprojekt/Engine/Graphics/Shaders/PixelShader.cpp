#include "PixelShader.h"
#include "../Renderer.h"
#include "../../Dev/Helpers.h"
#include "../../Dev/Log.h"

PixelShader::PixelShader(Renderer& renderer)
	: Shader(renderer),
	pixelShader(nullptr)
{
}

PixelShader::~PixelShader()
{
	S_RELEASE(this->pixelShader);
}

void PixelShader::loadPS(const std::string fileName)
{
	// Deallocate old object
	S_RELEASE(this->pixelShader);
}
