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

bool PixelShader::loadPS(const std::string fileName)
{
	// Deallocate old object
	S_RELEASE(this->pixelShader);

	// Load and fill with bytecode
	std::vector<char> psData;
	this->loadFile(fileName, psData);

	// Create pixel shader
	HRESULT result = Shader::getRenderer().getDevice()->CreatePixelShader(
		psData.data(), psData.size(), nullptr, &this->pixelShader
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed to create pixel shader.", result);

		return false;
	}

	return true;
}
