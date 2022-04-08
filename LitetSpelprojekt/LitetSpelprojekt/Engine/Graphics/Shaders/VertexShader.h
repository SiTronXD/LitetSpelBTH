#pragma once

#include "Shader.h"
#include "../InputLayoutDesc.h"

class VertexShader : public Shader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	bool createInputLayout(
		const std::vector<char>& vsByteCode,
		InputLayoutDesc& inputLayoutDesc
	);

public:
	VertexShader(Renderer& renderer);
	virtual ~VertexShader();

	bool loadVS(
		const std::string& fileName,
		InputLayoutDesc& inputLayoutDesc
	);

	inline ID3D11VertexShader*& getVS() { return this->vertexShader; }
	inline ID3D11InputLayout*& getInputLayout() { return this->inputLayout; }
};