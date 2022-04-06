#include "Shader.h"
#include "../../Dev/Log.h"
#include "../Renderer.h"

bool Shader::loadFile(
	const std::string& fileName, std::vector<char>& shaderDataOutput)
{
	// Open shader file
	std::ifstream shaderFile("CompiledShaders/" + fileName + ".cso", std::ios::binary);

	// Check if the file could not be found or opened
	if (shaderFile.fail())
	{
		Log::error("Failed to open shader file: " + fileName);

		return false;
	}

	// Fill with bytecode
	shaderDataOutput =
	{
		std::istreambuf_iterator<char>(shaderFile),
		std::istreambuf_iterator<char>()
	};

	return true;
}

Shader::Shader(Renderer& renderer)
	: renderer(renderer)
{
}

Shader::~Shader()
{
}