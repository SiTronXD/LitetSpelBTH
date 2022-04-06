#include "VertexShader.h"
#include "../Renderer.h"
#include "../../Dev/Helpers.h"
#include "../../Dev/Log.h"

bool VertexShader::createInputLayout(
	const std::vector<char>& vsByteCode, 
	InputLayoutDesc& inputLayoutDesc)
{
	// Create input layout
	HRESULT result = Shader::getRenderer().getDevice()->CreateInputLayout(
		&inputLayoutDesc.getInputElementDescs().front(),
		(UINT)inputLayoutDesc.getInputElementDescs().size(),
		vsByteCode.data(),
		vsByteCode.size(),
		&this->inputLayout
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed to create input layout.", result);

		return false;
	}

	return true;
}

VertexShader::VertexShader(Renderer& renderer)
	: Shader(renderer),
	vertexShader(nullptr),
	inputLayout(nullptr)
{
}

VertexShader::~VertexShader()
{
	S_RELEASE(this->vertexShader);
	S_RELEASE(this->inputLayout);
}

bool VertexShader::loadVS(
	const std::string& fileName,
	InputLayoutDesc& inputLayoutDesc)
{
	// Deallocate old objects
	S_RELEASE(this->vertexShader);
	S_RELEASE(this->inputLayout);

	// Load and fill with bytecode
	std::vector<char> vsData;
	this->loadFile(fileName, vsData);

	// Create vertex shader
	HRESULT result = Shader::getRenderer().getDevice()->CreateVertexShader(
		vsData.data(), vsData.size(), nullptr, &this->vertexShader
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed to create vertex shader.", result);

		return false;
	}

	// Finally create input layout
	bool couldCreateInputLayout = inputLayoutDesc.getInputElementDescs().size() > 0 ?
		this->createInputLayout(vsData, inputLayoutDesc) :
		true;

	return couldCreateInputLayout;
}
