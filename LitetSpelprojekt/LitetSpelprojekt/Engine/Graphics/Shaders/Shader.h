#pragma once

#include <string>
#include <vector>

#include <d3d11.h>

class Renderer;

class Shader
{
private:
	Renderer& renderer;

protected:
	bool loadFile(
		const std::string& fileName, std::vector<char>& shaderDataOutput
	);

	inline Renderer& getRenderer() { return this->renderer; }

public:
	Shader(Renderer& renderer);
	virtual ~Shader();
};